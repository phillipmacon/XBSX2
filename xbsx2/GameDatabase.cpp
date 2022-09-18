/*  XBSX2 - PS2 Emulator for Xbox Consoles  
 *  Copyright (C) 2002-2020  PCSX2 Dev Team
 * 
 *  XBSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  XBSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with XBSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PrecompiledHeader.h"

#include "GameDatabase.h"
#include "Host.h"
#include "Patch.h"
#include "vtlb.h"

#include "common/FileSystem.h"
#include "common/Path.h"
#include "common/StringUtil.h"
#include "common/Timer.h"

#include <sstream>
#include "ryml_std.hpp"
#include "ryml.hpp"
#include "fmt/core.h"
#include "fmt/ranges.h"
#include <fstream>
#include <mutex>
#include <optional>

namespace GameDatabaseSchema
{
	static const char* getHWFixName(GSHWFixId id);
	static std::optional<GSHWFixId> parseHWFixName(const std::string_view& name);
	static bool isUserHackHWFix(GSHWFixId id);
} // namespace GameDatabaseSchema

namespace GameDatabase
{
	static void parseAndInsert(const std::string_view& serial, const c4::yml::NodeRef& node);
	static bool checkAndLoad(const char* cached_filename, s64 expected_mtime);
	static bool initDatabase();
	static void load();
} // namespace GameDatabase

#include "svnrev.h"

static constexpr char GAMEDB_YAML_FILE_NAME[] = "GameIndex.yaml";

static constexpr char GAMEDB_CACHE_FILE_NAME[] = "gamedb.cache";
static constexpr u64 CACHE_FILE_MAGIC = UINT64_C(0x47414D4544423032); // GAMEDB03

static std::unordered_map<std::string, GameDatabaseSchema::GameEntry> s_game_db;
static std::once_flag s_load_once_flag;

std::string GameDatabaseSchema::GameEntry::memcardFiltersAsString() const
{
	return fmt::to_string(fmt::join(memcardFilters, "/"));
}

const std::string* GameDatabaseSchema::GameEntry::findPatch(u32 crc) const
{
	Console.WriteLn(fmt::format("[GameDB] Searching for patch with CRC '{:08X}'", crc));

	auto it = patches.find(crc);
	if (it != patches.end())
	{
		Console.WriteLn(fmt::format("[GameDB] Found patch with CRC '{:08X}'", crc));
		return &it->second;
	}

	it = patches.find(0);
	if (it != patches.end())
	{
		Console.WriteLn("[GameDB] Found and falling back to default patch");
		return &it->second;
	}
	Console.WriteLn("[GameDB] No CRC-specific patch or default patch found");
	return nullptr;
}

const char* GameDatabaseSchema::GameEntry::compatAsString() const
{
	switch (compat)
	{
		case GameDatabaseSchema::Compatibility::Perfect:
			return "Perfect";
		case GameDatabaseSchema::Compatibility::Playable:
			return "Playable";
		case GameDatabaseSchema::Compatibility::InGame:
			return "In-Game";
		case GameDatabaseSchema::Compatibility::Menu:
			return "Menu";
		case GameDatabaseSchema::Compatibility::Intro:
			return "Intro";
		case GameDatabaseSchema::Compatibility::Nothing:
			return "Nothing";
		default:
			return "Unknown";
	}
}

void GameDatabase::parseAndInsert(const std::string_view& serial, const c4::yml::NodeRef& node)
{
	GameDatabaseSchema::GameEntry gameEntry;
	if (node.has_child("name"))
	{
		node["name"] >> gameEntry.name;
	}
	if (node.has_child("region"))
	{
		node["region"] >> gameEntry.region;
	}
	if (node.has_child("compat"))
	{
		int val = 0;
		node["compat"] >> val;
		gameEntry.compat = static_cast<GameDatabaseSchema::Compatibility>(val);
	}
	if (node.has_child("roundModes"))
	{
		if (node["roundModes"].has_child("eeRoundMode"))
		{
			int eeVal = -1;
			node["roundModes"]["eeRoundMode"] >> eeVal;
			gameEntry.eeRoundMode = static_cast<GameDatabaseSchema::RoundMode>(eeVal);
		}
		if (node["roundModes"].has_child("vuRoundMode"))
		{
			int vuVal = -1;
			node["roundModes"]["vuRoundMode"] >> vuVal;
			gameEntry.vuRoundMode = static_cast<GameDatabaseSchema::RoundMode>(vuVal);
		}
	}
	if (node.has_child("clampModes"))
	{
		if (node["clampModes"].has_child("eeClampMode"))
		{
			int eeVal = -1;
			node["clampModes"]["eeClampMode"] >> eeVal;
			gameEntry.eeClampMode = static_cast<GameDatabaseSchema::ClampMode>(eeVal);
		}
		if (node["clampModes"].has_child("vuClampMode"))
		{
			int vuVal = -1;
			node["clampModes"]["vuClampMode"] >> vuVal;
			gameEntry.vuClampMode = static_cast<GameDatabaseSchema::ClampMode>(vuVal);
		}
	}

	// Validate game fixes, invalid ones will be dropped!
	if (node.has_child("gameFixes") && node["gameFixes"].has_children())
	{
		for (const ryml::NodeRef& n : node["gameFixes"].children())
		{
			bool fixValidated = false;
			auto fix = std::string(n.val().str, n.val().len);

			// Enum values don't end with Hack, but gamedb does, so remove it before comparing.
			if (StringUtil::EndsWith(fix, "Hack"))
			{
				fix.erase(fix.size() - 4);
				for (GamefixId id = GamefixId_FIRST; id < pxEnumEnd; ++id)
				{
					if (fix.compare(EnumToString(id)) == 0 &&
						std::find(gameEntry.gameFixes.begin(), gameEntry.gameFixes.end(), id) == gameEntry.gameFixes.end())
					{
						gameEntry.gameFixes.push_back(id);
						fixValidated = true;
						break;
					}
				}
			}

			if (!fixValidated)
			{
				Console.Error(fmt::format("[GameDB] Invalid gamefix: '{}', specified for serial: '{}'. Dropping!", fix, serial));
			}
		}
	}

	// Validate speed hacks, invalid ones will be dropped!
	if (node.has_child("speedHacks") && node["speedHacks"].has_children())
	{
		for (const ryml::NodeRef& n : node["speedHacks"].children())
		{
			bool speedHackValidated = false;
			auto speedHack = std::string(n.key().str, n.key().len);

			// Same deal with SpeedHacks
			if (StringUtil::EndsWith(speedHack, "SpeedHack"))
			{
				speedHack.erase(speedHack.size() - 9);
				for (SpeedhackId id = SpeedhackId_FIRST; id < pxEnumEnd; ++id)
				{
					if (speedHack.compare(EnumToString(id)) == 0 &&
						std::none_of(gameEntry.speedHacks.begin(), gameEntry.speedHacks.end(), [id](const auto& it) { return it.first == id; }))
					{
						gameEntry.speedHacks.emplace_back(id, std::atoi(n.val().str));
						speedHackValidated = true;
						break;
					}
				}
			}

			if (!speedHackValidated)
			{
				Console.Error(fmt::format("[GameDB] Invalid speedhack: '{}', specified for serial: '{}'. Dropping!", speedHack.c_str(), serial));
			}
		}
	}

	if (node.has_child("gsHWFixes"))
	{
		for (const ryml::NodeRef& n : node["gsHWFixes"].children())
		{
			const std::string_view id_name(n.key().data(), n.key().size());
			std::optional<GameDatabaseSchema::GSHWFixId> id = GameDatabaseSchema::parseHWFixName(id_name);
			std::optional<s32> value = n.has_val() ? StringUtil::FromChars<s32>(std::string_view(n.val().data(), n.val().size())) : 1;
			if (!id.has_value() || !value.has_value())
			{
				Console.Error("[GameDB] Invalid GS HW Fix: '%.*s' specified for serial '%.*s'. Dropping!",
					static_cast<int>(id_name.size()), id_name.data(),
					static_cast<int>(serial.size()), serial.data());
				continue;
			}

			gameEntry.gsHWFixes.emplace_back(id.value(), value.value());
		}
	}

	// Memory Card Filters - Store as a vector to allow flexibility in the future
	// - currently they are used as a '\n' delimited string in the app
	if (node.has_child("memcardFilters") && node["memcardFilters"].has_children())
	{
		for (const ryml::NodeRef& n : node["memcardFilters"].children())
		{
			auto memcardFilter = std::string(n.val().str, n.val().len);
			gameEntry.memcardFilters.emplace_back(std::move(memcardFilter));
		}
	}

	// Game Patches
	if (node.has_child("patches") && node["patches"].has_children())
	{
		for (const ryml::NodeRef& n : node["patches"].children())
		{
			// use a crc of 0 for default patches
			const std::string_view crc_str(n.key().str, n.key().len);
			const std::optional<u32> crc = (StringUtil::compareNoCase(crc_str, "default")) ? std::optional<u32>(0) : StringUtil::FromChars<u32>(crc_str, 16);
			if (!crc.has_value())
			{
				Console.Error(fmt::format("[GameDB] Invalid CRC '{}' found for serial: '{}'. Skipping!", crc_str, serial));
				continue;
			}
			if (gameEntry.patches.find(crc.value()) != gameEntry.patches.end())
			{
				Console.Error(fmt::format("[GameDB] Duplicate CRC '{}' found for serial: '{}'. Skipping, CRCs are case-insensitive!", crc_str, serial));
				continue;
			}

			std::string patch;
			if (n.has_child("content"))
				n["content"] >> patch;
			gameEntry.patches.emplace(crc.value(), std::move(patch));
		}
	}

	s_game_db.emplace(std::move(serial), std::move(gameEntry));
}

static const char* s_gs_hw_fix_names[] = {
	"autoFlush",
	"cpuFramebufferConversion",
	"disableDepthSupport",
	"wrapGSMem",
	"preloadFrameData",
	"disablePartialInvalidation",
	"textureInsideRT",
	"alignSprite",
	"mergeSprite",
	"wildArmsHack",
	"pointListPalette",
	"mipmap",
	"trilinearFiltering",
	"skipDrawStart",
	"skipDrawEnd",
	"halfBottomOverride",
	"halfPixelOffset",
	"roundSprite",
	"texturePreloading",
	"deinterlace",
	"cpuSpriteRenderBW",
	"textureBarriers",
};
static_assert(std::size(s_gs_hw_fix_names) == static_cast<u32>(GameDatabaseSchema::GSHWFixId::Count), "HW fix name lookup is correct size");

const char* GameDatabaseSchema::getHWFixName(GSHWFixId id)
{
	return s_gs_hw_fix_names[static_cast<u32>(id)];
}

static std::optional<GameDatabaseSchema::GSHWFixId> GameDatabaseSchema::parseHWFixName(const std::string_view& name)
{
	for (u32 i = 0; i < std::size(s_gs_hw_fix_names); i++)
	{
		if (name.compare(s_gs_hw_fix_names[i]) == 0)
			return static_cast<GameDatabaseSchema::GSHWFixId>(i);
	}

	return std::nullopt;
}

bool GameDatabaseSchema::isUserHackHWFix(GSHWFixId id)
{
	switch (id)
	{
		case GSHWFixId::Deinterlace:
		case GSHWFixId::Mipmap:
		case GSHWFixId::TexturePreloading:
		case GSHWFixId::TextureBarriers:
		case GSHWFixId::PointListPalette:
			return false;

#ifdef XBSX2_CORE
			// Trifiltering isn't a hack in Qt.
		case GSHWFixId::TrilinearFiltering:
			return false;
#endif

		default:
			return true;
	}
}

u32 GameDatabaseSchema::GameEntry::applyGameFixes(Xbsx2Config& config, bool applyAuto) const
{
	// Only apply core game fixes if the user has enabled them.
	if (!applyAuto)
		Console.Warning("[GameDB] Game Fixes are disabled");

	u32 num_applied_fixes = 0;

	if (eeRoundMode != GameDatabaseSchema::RoundMode::Undefined)
	{
		const SSE_RoundMode eeRM = (SSE_RoundMode)enum_cast(eeRoundMode);
		if (EnumIsValid(eeRM))
		{
			if (applyAuto)
			{
				PatchesCon->WriteLn("(GameDB) Changing EE/FPU roundmode to %d [%s]", eeRM, EnumToString(eeRM));
				config.Cpu.sseMXCSR.SetRoundMode(eeRM);
				num_applied_fixes++;
			}
			else
				PatchesCon->Warning("[GameDB] Skipping changing EE/FPU roundmode to %d [%s]", eeRM, EnumToString(eeRM));
		}
	}

	if (vuRoundMode != GameDatabaseSchema::RoundMode::Undefined)
	{
		const SSE_RoundMode vuRM = (SSE_RoundMode)enum_cast(vuRoundMode);
		if (EnumIsValid(vuRM))
		{
			if (applyAuto)
			{
				PatchesCon->WriteLn("(GameDB) Changing VU0/VU1 roundmode to %d [%s]", vuRM, EnumToString(vuRM));
				config.Cpu.sseVUMXCSR.SetRoundMode(vuRM);
				num_applied_fixes++;
			}
			else
				PatchesCon->Warning("[GameDB] Skipping changing VU0/VU1 roundmode to %d [%s]", vuRM, EnumToString(vuRM));
		}
	}

	if (eeClampMode != GameDatabaseSchema::ClampMode::Undefined)
	{
		const int clampMode = enum_cast(eeClampMode);
		if (applyAuto)
		{
			PatchesCon->WriteLn("(GameDB) Changing EE/FPU clamp mode [mode=%d]", clampMode);
			config.Cpu.Recompiler.fpuOverflow = (clampMode >= 1);
			config.Cpu.Recompiler.fpuExtraOverflow = (clampMode >= 2);
			config.Cpu.Recompiler.fpuFullMode = (clampMode >= 3);
			num_applied_fixes++;
		}
		else
			PatchesCon->Warning("[GameDB] Skipping changing EE/FPU clamp mode [mode=%d]", clampMode);
	}

	if (vuClampMode != GameDatabaseSchema::ClampMode::Undefined)
	{
		const int clampMode = enum_cast(vuClampMode);
		if (applyAuto)
		{
			PatchesCon->WriteLn("(GameDB) Changing VU0/VU1 clamp mode [mode=%d]", clampMode);
			config.Cpu.Recompiler.vuOverflow = (clampMode >= 1);
			config.Cpu.Recompiler.vuExtraOverflow = (clampMode >= 2);
			config.Cpu.Recompiler.vuSignOverflow = (clampMode >= 3);
			num_applied_fixes++;
		}
		else
			PatchesCon->Warning("[GameDB] Skipping changing VU0/VU1 clamp mode [mode=%d]", clampMode);
	}

	// TODO - config - this could be simplified with maps instead of bitfields and enums
	for (const auto& it : speedHacks)
	{
		const bool mode = it.second != 0;
		if (!applyAuto)
		{
			PatchesCon->Warning("[GameDB] Skipping setting Speedhack '%s' to [mode=%d]", EnumToString(it.first), mode);
			continue;
		}
		// Legacy note - speedhacks are setup in the GameDB as integer values, but
		// are effectively booleans like the gamefixes
		config.Speedhacks.Set(it.first, mode);
		PatchesCon->WriteLn("(GameDB) Setting Speedhack '%s' to [mode=%d]", EnumToString(it.first), mode);
		num_applied_fixes++;
	}

	// TODO - config - this could be simplified with maps instead of bitfields and enums
	for (const GamefixId id : gameFixes)
	{
		if (!applyAuto)
		{
			PatchesCon->Warning("[GameDB] Skipping Gamefix: %s", EnumToString(id));
			continue;
		}
		// if the fix is present, it is said to be enabled
		config.Gamefixes.Set(id, true);
		PatchesCon->WriteLn("(GameDB) Enabled Gamefix: %s", EnumToString(id));
		num_applied_fixes++;

		// The LUT is only used for 1 game so we allocate it only when the gamefix is enabled (save 4MB)
		if (id == Fix_GoemonTlbMiss && true)
			vtlb_Alloc_Ppmap();
	}

	return num_applied_fixes;
}

bool GameDatabaseSchema::GameEntry::configMatchesHWFix(const Xbsx2Config::GSOptions& config, GSHWFixId id, int value) const
{
	switch (id)
	{
		case GSHWFixId::AutoFlush:
			return (static_cast<int>(config.UserHacks_AutoFlush) == value);

		case GSHWFixId::CPUFramebufferConversion:
			return (static_cast<int>(config.UserHacks_CPUFBConversion) == value);

		case GSHWFixId::DisableDepthSupport:
			return (static_cast<int>(config.UserHacks_DisableDepthSupport) == value);

		case GSHWFixId::WrapGSMem:
			return (static_cast<int>(config.WrapGSMem) == value);

		case GSHWFixId::PreloadFrameData:
			return (static_cast<int>(config.PreloadFrameWithGSData) == value);

		case GSHWFixId::DisablePartialInvalidation:
			return (static_cast<int>(config.UserHacks_DisablePartialInvalidation) == value);

		case GSHWFixId::TextureInsideRT:
			return (static_cast<int>(config.UserHacks_TextureInsideRt) == value);

		case GSHWFixId::AlignSprite:
			return (config.UpscaleMultiplier == 1 || static_cast<int>(config.UserHacks_AlignSpriteX) == value);

		case GSHWFixId::MergeSprite:
			return (config.UpscaleMultiplier == 1 || static_cast<int>(config.UserHacks_MergePPSprite) == value);

		case GSHWFixId::WildArmsHack:
			return (config.UpscaleMultiplier == 1 || static_cast<int>(config.UserHacks_WildHack) == value);

		case GSHWFixId::PointListPalette:
			return (static_cast<int>(config.PointListPalette) == value);

		case GSHWFixId::Mipmap:
			return (config.HWMipmap == HWMipmapLevel::Automatic || static_cast<int>(config.HWMipmap) == value);

		case GSHWFixId::TrilinearFiltering:
			return (config.UserHacks_TriFilter == TriFiltering::Automatic || static_cast<int>(config.UserHacks_TriFilter) == value);

		case GSHWFixId::SkipDrawStart:
			return (config.SkipDrawStart == value);

		case GSHWFixId::SkipDrawEnd:
			return (config.SkipDrawEnd == value);

		case GSHWFixId::HalfBottomOverride:
			return (config.UserHacks_HalfBottomOverride == value);

		case GSHWFixId::HalfPixelOffset:
			return (config.UpscaleMultiplier == 1 || config.UserHacks_HalfPixelOffset == value);

		case GSHWFixId::RoundSprite:
			return (config.UpscaleMultiplier == 1 || config.UserHacks_RoundSprite == value);

		case GSHWFixId::TexturePreloading:
			return (static_cast<int>(config.TexturePreloading) <= value);

		case GSHWFixId::Deinterlace:
			return (config.InterlaceMode == GSInterlaceMode::Automatic || static_cast<int>(config.InterlaceMode) == value);

		case GSHWFixId::CPUSpriteRenderBW:
			return (config.UserHacks_CPUSpriteRenderBW == value);

		case GSHWFixId::TextureBarriers:
			return (config.OverrideTextureBarriers < 0 || config.OverrideTextureBarriers == value);

		default:
			return false;
	}
}

u32 GameDatabaseSchema::GameEntry::applyGSHardwareFixes(Xbsx2Config::GSOptions& config) const
{
	std::string disabled_fixes;

	// Only apply GS HW fixes if the user hasn't manually enabled HW fixes.
	const bool apply_auto_fixes = !config.ManualUserHacks;
	if (!apply_auto_fixes)
		Console.Warning("[GameDB] Manual GS hardware renderer fixes are enabled, not using automatic hardware renderer fixes from GameDB.");

	u32 num_applied_fixes = 0;
	for (const auto& [id, value] : gsHWFixes)
	{
		if (isUserHackHWFix(id) && !apply_auto_fixes)
		{
			if (configMatchesHWFix(config, id, value))
				continue;

			PatchesCon->Warning("[GameDB] Skipping GS Hardware Fix: %s to [mode=%d]", getHWFixName(id), value);
			fmt::format_to(std::back_inserter(disabled_fixes), "{} {} = {}", disabled_fixes.empty() ? " " : "\n ", getHWFixName(id), value);
			continue;
		}

		switch (id)
		{
			case GSHWFixId::AutoFlush:
				config.UserHacks_AutoFlush = (value > 0);
				break;

			case GSHWFixId::CPUFramebufferConversion:
				config.UserHacks_CPUFBConversion = (value > 0);
				break;

			case GSHWFixId::DisableDepthSupport:
				config.UserHacks_DisableDepthSupport = (value > 0);
				break;

			case GSHWFixId::WrapGSMem:
				config.WrapGSMem = (value > 0);
				break;

			case GSHWFixId::PreloadFrameData:
				config.PreloadFrameWithGSData = (value > 0);
				break;

			case GSHWFixId::DisablePartialInvalidation:
				config.UserHacks_DisablePartialInvalidation = (value > 0);
				break;

			case GSHWFixId::TextureInsideRT:
				config.UserHacks_TextureInsideRt = (value > 0);
				break;

			case GSHWFixId::AlignSprite:
				config.UserHacks_AlignSpriteX = (value > 0);
				break;

			case GSHWFixId::MergeSprite:
				config.UserHacks_MergePPSprite = (value > 0);
				break;

			case GSHWFixId::WildArmsHack:
				config.UserHacks_WildHack = (value > 0);
				break;

			case GSHWFixId::PointListPalette:
				config.PointListPalette = (value > 0);
				break;

			case GSHWFixId::Mipmap:
			{
				if (value >= 0 && value <= static_cast<int>(HWMipmapLevel::Full))
				{
					if (config.HWMipmap == HWMipmapLevel::Automatic)
						config.HWMipmap = static_cast<HWMipmapLevel>(value);
					else if (config.HWMipmap == HWMipmapLevel::Off)
						Console.Warning("[GameDB] Game requires mipmapping but it has been force disabled.");
				}
			}
			break;

			case GSHWFixId::TrilinearFiltering:
			{
				if (value >= 0 && value <= static_cast<int>(TriFiltering::Forced))
				{
					if (config.UserHacks_TriFilter == TriFiltering::Automatic)
						config.UserHacks_TriFilter = static_cast<TriFiltering>(value);
					else if (config.UserHacks_TriFilter == TriFiltering::Off)
						Console.Warning("[GameDB] Game requires trilinear filtering but it has been force disabled.");
				}
			}
			break;

			case GSHWFixId::SkipDrawStart:
				config.SkipDrawStart = value;
				break;

			case GSHWFixId::SkipDrawEnd:
				config.SkipDrawEnd = value;
				break;

			case GSHWFixId::HalfBottomOverride:
				config.UserHacks_HalfBottomOverride = value;
				break;

			case GSHWFixId::HalfPixelOffset:
				config.UserHacks_HalfPixelOffset = value;
				break;

			case GSHWFixId::RoundSprite:
				config.UserHacks_RoundSprite = value;
				break;

			case GSHWFixId::TexturePreloading:
			{
				if (value >= 0 && value <= static_cast<int>(TexturePreloadingLevel::Full))
					config.TexturePreloading = std::min(config.TexturePreloading, static_cast<TexturePreloadingLevel>(value));
			}
			break;

			case GSHWFixId::Deinterlace:
			{
				if (value >= 0 && value <= static_cast<int>(GSInterlaceMode::Automatic))
				{
					if (config.InterlaceMode == GSInterlaceMode::Automatic)
						config.InterlaceMode = static_cast<GSInterlaceMode>(value);
					else
						Console.Warning("[GameDB] Game requires different deinterlace mode but it has been overridden by user setting.");
				}
			}
			break;

			case GSHWFixId::CPUSpriteRenderBW:
				config.UserHacks_CPUSpriteRenderBW = value;
				break;

			case GSHWFixId::TextureBarriers:
			{
				if (value >= 0 && value <= 2 && config.OverrideTextureBarriers < 0)
					config.OverrideTextureBarriers = value;
			}
			break;

			default:
				break;
		}

		PatchesCon->WriteLn("[GameDB] Enabled GS Hardware Fix: %s to [mode=%d]", getHWFixName(id), value);
		num_applied_fixes++;
	}

	// fixup skipdraw range just in case the db has a bad range (but the linter should catch this)
	config.SkipDrawEnd = std::max(config.SkipDrawStart, config.SkipDrawEnd);

#ifdef XBSX2_CORE
	if (!disabled_fixes.empty())
	{
		Host::AddKeyedOSDMessage("HWFixesWarning",
			fmt::format("Manual GS hardware renderer fixes are enabled, automatic fixes were not applied:\n{}",
				disabled_fixes),
			3.0f);
	}
	else
	{
		Host::RemoveKeyedOSDMessage("HWFixesWarning");
	}
#endif

	return num_applied_fixes;
}

bool GameDatabase::initDatabase()
{
	ryml::Callbacks rymlCallbacks = ryml::get_callbacks();
	rymlCallbacks.m_error = [](const char* msg, size_t msg_len, ryml::Location loc, void*) {
		throw std::runtime_error(fmt::format("[YAML] Parsing error at {}:{} (bufpos={}): {}",
			loc.line, loc.col, loc.offset, msg));
	};
	ryml::set_callbacks(rymlCallbacks);
	c4::set_error_callback([](const char* msg, size_t msg_size) {
		throw std::runtime_error(fmt::format("[YAML] Internal Parsing error: {}",
			msg));
	});
	try
	{
		auto buf = Host::ReadResourceFileToString(GAMEDB_YAML_FILE_NAME);
		if (!buf.has_value())
		{
			Console.Error("[GameDB] Unable to open GameDB file, file does not exist.");
			return false;
		}

		ryml::Tree tree = ryml::parse_in_arena(c4::to_csubstr(buf.value()));
		ryml::NodeRef root = tree.rootref();

		for (const ryml::NodeRef& n : root.children())
		{
			auto serial = StringUtil::toLower(std::string(n.key().str, n.key().len));

			// Serials and CRCs must be inserted as lower-case, as that is how they are retrieved
			// this is because the application may pass a lowercase CRC or serial along
			//
			// However, YAML's keys are as expected case-sensitive, so we have to explicitly do our own duplicate checking
			if (s_game_db.count(serial) == 1)
			{
				Console.Error(fmt::format("[GameDB] Duplicate serial '{}' found in GameDB. Skipping, Serials are case-insensitive!", serial));
				continue;
			}
			if (n.is_map())
			{
				parseAndInsert(serial, n);
			}
		}

		ryml::reset_callbacks();
		return true;
	}
	catch (const std::exception& e)
	{
		Console.Error(fmt::format("[GameDB] Error occured when initializing GameDB: {}", e.what()));
		ryml::reset_callbacks();
		return false;
	}
}

static bool ReadString(std::FILE* stream, std::string* dest)
{
	u32 size;
	if (std::fread(&size, sizeof(size), 1, stream) != 1)
		return false;

	dest->resize(size);
	if (size > 0 && std::fread(dest->data(), size, 1, stream) != 1)
		return false;

	return true;
}

static bool ReadS8(std::FILE* stream, s8* dest)
{
	return std::fread(dest, sizeof(s8), 1, stream) > 0;
}

static bool ReadU8(std::FILE* stream, u8* dest)
{
	return std::fread(dest, sizeof(u8), 1, stream) > 0;
}

static bool ReadS32(std::FILE* stream, s32* dest)
{
	return std::fread(dest, sizeof(s32), 1, stream) > 0;
}

static bool ReadU32(std::FILE* stream, u32* dest)
{
	return std::fread(dest, sizeof(u32), 1, stream) > 0;
}

static bool ReadS64(std::FILE* stream, s64* dest)
{
	return std::fread(dest, sizeof(s64), 1, stream) > 0;
}

static bool ReadU64(std::FILE* stream, u64* dest)
{
	return std::fread(dest, sizeof(u64), 1, stream) > 0;
}

static bool WriteString(std::FILE* stream, const std::string& str)
{
	const u32 size = static_cast<u32>(str.size());
	return (std::fwrite(&size, sizeof(size), 1, stream) > 0 &&
			(size == 0 || std::fwrite(str.data(), size, 1, stream) > 0));
}

static bool WriteS8(std::FILE* stream, s8 dest)
{
	return std::fwrite(&dest, sizeof(s8), 1, stream) > 0;
}

static bool WriteU8(std::FILE* stream, u8 dest)
{
	return std::fwrite(&dest, sizeof(u8), 1, stream) > 0;
}

static bool WriteS32(std::FILE* stream, s32 dest)
{
	return std::fwrite(&dest, sizeof(s32), 1, stream) > 0;
}

static bool WriteU32(std::FILE* stream, u32 dest)
{
	return std::fwrite(&dest, sizeof(u32), 1, stream) > 0;
}

static bool WriteS64(std::FILE* stream, s64 dest)
{
	return std::fwrite(&dest, sizeof(s64), 1, stream) > 0;
}

static bool WriteU64(std::FILE* stream, u64 dest)
{
	return std::fwrite(&dest, sizeof(u64), 1, stream) > 0;
}

static s64 GetExpectedMTime()
{
	return Host::GetResourceFileTimestamp(GAMEDB_YAML_FILE_NAME).value_or(-1);
}

bool GameDatabase::checkAndLoad(const char* cached_filename, s64 expected_mtime)
{
	auto fp = FileSystem::OpenManagedCFile(cached_filename, "rb");
	if (!fp)
		return false;

	u64 file_signature;
	s64 file_mtime, start_pos, file_size;
	std::string file_version;
	if (!ReadU64(fp.get(), &file_signature) || file_signature != CACHE_FILE_MAGIC ||
		!ReadS64(fp.get(), &file_mtime) || file_mtime != expected_mtime ||
		!ReadString(fp.get(), &file_version) || file_version != GIT_REV ||
		(start_pos = FileSystem::FTell64(fp.get())) < 0 || FileSystem::FSeek64(fp.get(), 0, SEEK_END) != 0 ||
		(file_size = FileSystem::FTell64(fp.get())) < 0 || FileSystem::FSeek64(fp.get(), start_pos, SEEK_SET) != 0)
	{
		return false;
	}

	while (FileSystem::FTell64(fp.get()) != file_size)
	{
		std::string serial;
		GameDatabaseSchema::GameEntry entry;
		u8 compat;
		s8 ee_round, ee_clamp, vu_round, vu_clamp;
		u32 game_fix_count, gs_hw_fix_count, speed_hack_count, memcard_filter_count, patch_count;

		if (!ReadString(fp.get(), &serial) ||
			!ReadString(fp.get(), &entry.name) ||
			!ReadString(fp.get(), &entry.region) ||
			!ReadU8(fp.get(), &compat) || compat > static_cast<u8>(GameDatabaseSchema::Compatibility::Perfect) ||
			!ReadS8(fp.get(), &ee_round) || ee_round < static_cast<s8>(GameDatabaseSchema::RoundMode::Undefined) || ee_round > static_cast<s8>(GameDatabaseSchema::RoundMode::ChopZero) ||
			!ReadS8(fp.get(), &ee_clamp) || ee_clamp < static_cast<s8>(GameDatabaseSchema::ClampMode::Undefined) || ee_clamp > static_cast<s8>(GameDatabaseSchema::ClampMode::Full) ||
			!ReadS8(fp.get(), &vu_round) || vu_round < static_cast<s8>(GameDatabaseSchema::RoundMode::Undefined) || vu_round > static_cast<s8>(GameDatabaseSchema::RoundMode::ChopZero) ||
			!ReadS8(fp.get(), &vu_clamp) || vu_clamp < static_cast<s8>(GameDatabaseSchema::ClampMode::Undefined) || vu_clamp > static_cast<s8>(GameDatabaseSchema::ClampMode::Full) ||
			!ReadU32(fp.get(), &game_fix_count) ||
			!ReadU32(fp.get(), &gs_hw_fix_count) ||
			!ReadU32(fp.get(), &speed_hack_count) ||
			!ReadU32(fp.get(), &memcard_filter_count) ||
			!ReadU32(fp.get(), &patch_count))
		{
			Console.Error("GameDB: Read error while loading entry");
			return false;
		}

		entry.compat = static_cast<GameDatabaseSchema::Compatibility>(compat);
		entry.eeRoundMode = static_cast<GameDatabaseSchema::RoundMode>(ee_round);
		entry.eeClampMode = static_cast<GameDatabaseSchema::ClampMode>(ee_clamp);
		entry.vuRoundMode = static_cast<GameDatabaseSchema::RoundMode>(vu_round);
		entry.vuClampMode = static_cast<GameDatabaseSchema::ClampMode>(vu_clamp);

		entry.gameFixes.reserve(game_fix_count);
		for (u32 i = 0; i < game_fix_count; i++)
		{
			u32 game_fix_id;
			if (!ReadU32(fp.get(), &game_fix_id) || game_fix_id >= static_cast<u32>(GamefixId_COUNT))
				return false;

			entry.gameFixes.push_back(static_cast<GamefixId>(game_fix_id));
		}

		entry.gsHWFixes.reserve(gs_hw_fix_count);
		for (u32 i = 0; i < gs_hw_fix_count; i++)
		{
			u32 gs_hw_fix_id;
			s32 gs_hw_fix_value;
			if (!ReadU32(fp.get(), &gs_hw_fix_id) || !ReadS32(fp.get(), &gs_hw_fix_value) || gs_hw_fix_id >= static_cast<u32>(GameDatabaseSchema::GSHWFixId::Count))
				return false;

			entry.gsHWFixes.emplace_back(static_cast<GameDatabaseSchema::GSHWFixId>(gs_hw_fix_id), gs_hw_fix_value);
		}

		for (u32 i = 0; i < speed_hack_count; i++)
		{
			u32 speed_hack_id;
			s32 speed_hack_value;
			if (!ReadU32(fp.get(), &speed_hack_id) || !ReadS32(fp.get(), &speed_hack_value) || speed_hack_id >= static_cast<u32>(SpeedhackId_COUNT))
				return false;
			entry.speedHacks.emplace_back(static_cast<SpeedhackId>(speed_hack_id), speed_hack_value);
		}

		entry.memcardFilters.resize(memcard_filter_count);
		for (u32 i = 0; i < memcard_filter_count; i++)
		{
			if (!ReadString(fp.get(), &entry.memcardFilters[i]))
				return false;
		}

		for (u32 i = 0; i < patch_count; i++)
		{
			u32 patch_crc;
			std::string patch_content;
			if (!ReadU32(fp.get(), &patch_crc) || !ReadString(fp.get(), &patch_content))
				return false;

			entry.patches.emplace(std::move(patch_crc), std::move(patch_content));
		}

		s_game_db.emplace(std::move(serial), std::move(entry));
	}

	return true;
}

static bool SaveCache(const char* cached_filename, s64 mtime)
{
	auto fp = FileSystem::OpenManagedCFile(cached_filename, "wb");
	if (!fp)
		return false;

	if (!WriteU64(fp.get(), CACHE_FILE_MAGIC) || !WriteS64(fp.get(), mtime) || !WriteString(fp.get(), GIT_REV))
		return false;

	for (const auto& it : s_game_db)
	{
		const GameDatabaseSchema::GameEntry& entry = it.second;
		const u8 compat = static_cast<u8>(entry.compat);
		const s8 ee_round = static_cast<s8>(entry.eeRoundMode);
		const s8 ee_clamp = static_cast<s8>(entry.eeClampMode);
		const s8 vu_round = static_cast<s8>(entry.vuRoundMode);
		const s8 vu_clamp = static_cast<s8>(entry.vuClampMode);

		if (!WriteString(fp.get(), it.first) ||
			!WriteString(fp.get(), entry.name) ||
			!WriteString(fp.get(), entry.region) ||
			!WriteU8(fp.get(), compat) ||
			!WriteS8(fp.get(), ee_round) ||
			!WriteS8(fp.get(), ee_clamp) ||
			!WriteS8(fp.get(), vu_round) ||
			!WriteS8(fp.get(), vu_clamp) ||
			!WriteU32(fp.get(), static_cast<u32>(entry.gameFixes.size())) ||
			!WriteU32(fp.get(), static_cast<u32>(entry.gsHWFixes.size())) ||
			!WriteU32(fp.get(), static_cast<u32>(entry.speedHacks.size())) ||
			!WriteU32(fp.get(), static_cast<u32>(entry.memcardFilters.size())) ||
			!WriteU32(fp.get(), static_cast<u32>(entry.patches.size())))
		{
			return false;
		}

		for (const GamefixId it : entry.gameFixes)
		{
			if (!WriteU32(fp.get(), static_cast<u32>(it)))
				return false;
		}

		for (const auto& it : entry.gsHWFixes)
		{
			if (!WriteU32(fp.get(), static_cast<u32>(it.first)) || !WriteS32(fp.get(), it.second))
				return false;
		}

		for (const auto& it : entry.speedHacks)
		{
			if (!WriteU32(fp.get(), static_cast<u32>(it.first)) || !WriteS32(fp.get(), it.second))
				return false;
		}

		for (const std::string& it : entry.memcardFilters)
		{
			if (!WriteString(fp.get(), it))
				return false;
		}

		for (const auto& it : entry.patches)
		{
			if (!WriteU32(fp.get(), it.first) || !WriteString(fp.get(), it.second))
				return false;
		}
	}

	return std::fflush(fp.get()) == 0;
}

void GameDatabase::load()
{
	const std::string cache_filename(Path::Combine(EmuFolders::Cache, GAMEDB_CACHE_FILE_NAME));
	const s64 expected_mtime = GetExpectedMTime();

	Common::Timer timer;

	if (!FileSystem::FileExists(cache_filename.c_str()) || !checkAndLoad(cache_filename.c_str(), expected_mtime))
	{
		Console.Warning("GameDB cache file does not exist or failed validation, recreating");
		s_game_db.clear();

		if (!initDatabase())
		{
			Console.Error("GameDB: Failed to load YAML file");
			return;
		}

		if (!SaveCache(cache_filename.c_str(), expected_mtime))
			Console.Error("GameDB: Failed to save new cache");
	}

	Console.WriteLn("[GameDB] %zu games on record (loaded in %.2fms)", s_game_db.size(), timer.GetTimeMilliseconds());
}

void GameDatabase::ensureLoaded()
{
	std::call_once(s_load_once_flag, []() {
		load();
	});
}

const GameDatabaseSchema::GameEntry* GameDatabase::findGame(const std::string_view& serial)
{
	GameDatabase::ensureLoaded();

	std::string serialLower = StringUtil::toLower(serial);
	Console.WriteLn(fmt::format("[GameDB] Searching for '{}' in GameDB", serialLower));
	const auto gameEntry = s_game_db.find(serialLower);
	if (gameEntry != s_game_db.end())
	{
		Console.WriteLn(fmt::format("[GameDB] Found '{}' in GameDB", serialLower));
		return &gameEntry->second;
	}

	Console.Error(fmt::format("[GameDB] Could not find '{}' in GameDB", serialLower));
	return nullptr;
}
