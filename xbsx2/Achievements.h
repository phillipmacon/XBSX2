#pragma once
#include "common/Xbsx2Types.h"
#include <vector>

namespace Achievements
{
#ifdef ENABLE_ACHIEVEMENTS

	// Implemented in Host.
	extern bool Reset();
	extern void LoadState(const u8* state_data, u32 state_data_size);
	extern std::vector<u8> SaveState();
	extern void GameChanged(u32 crc);

	/// Re-enables hardcode mode if it is enabled in the settings.
	extern void ResetChallengeMode();

	/// Forces hardcore mode off until next reset.
	extern void DisableChallengeMode();

	/// Prompts the user to disable hardcore mode, if they agree, returns true.
	extern bool ConfirmChallengeModeDisable(const char* trigger);

	/// Returns true if features such as save states should be disabled.
	extern bool ChallengeModeActive();

#else

	// Make noops when compiling without cheevos.
	static inline bool Reset()
	{
		return true;
	}
	static inline void LoadState(const u8* state_data, u32 state_data_size)
	{
	}
	static inline std::vector<u8> SaveState()
	{
		return {};
	}
	static inline void GameChanged()
	{
	}

	static constexpr inline bool ChallengeModeActive()
	{
		return false;
	}

	static inline void ResetChallengeMode() {}

	static inline void DisableChallengeMode() {}

	static inline bool ConfirmChallengeModeDisable(const char* trigger)
	{
		return true;
	}

#endif
} // namespace Achievements
