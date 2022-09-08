add_executable(pcsx2-nogui)

if (PACKAGE_MODE)
	install(TARGETS pcsx2-nogui DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
	install(TARGETS pcsx2-nogui DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endif()

target_sources(pcsx2-nogui PRIVATE
	NoGUIHost.cpp
	NoGUIHost.h
	NoGUIPlatform.h
)

target_precompile_headers(pcsx2-nogui PRIVATE PrecompiledHeader.h)

target_include_directories(pcsx2-nogui PRIVATE
	"${CMAKE_BINARY_DIR}/common/include"
	"${CMAKE_SOURCE_DIR}/pcsx2"
	"${CMAKE_SOURCE_DIR}/pcsx2-nogui"
)

target_link_libraries(pcsx2-nogui PRIVATE
	PCSX2_FLAGS
	PCSX2
)

if (X11_API)
	message(STATUS "Building X11 NoGUI Platform.")
	target_compile_definitions(pcsx2-nogui PRIVATE
		NOGUI_PLATFORM_X11
	)
	target_sources(pcsx2-nogui PRIVATE
		X11NoGUIPlatform.cpp
		X11NoGUIPlatform.h
	)
	target_include_directories(pcsx2-nogui PRIVATE
		"${X11_INCLUDE_DIR}"
	)
	target_link_libraries(pcsx2-nogui PRIVATE
		"${X11_LIBRARIES}"
	)
endif()

if (WAYLAND_API)
	message(STATUS "Building Wayland NoGUI Platform.")
	# Generate the xdg-shell and xdg-decoration protocols at build-time.
	# Because these are C, not C++, we have to put them in their own library, otherwise
	# cmake tries to generate a C PCH as well as the C++ one...
	ecm_add_wayland_client_protocol(WAYLAND_PLATFORM_SRCS
		PROTOCOL "${WAYLAND_PROTOCOLS_PKGDATADIR}/stable/xdg-shell/xdg-shell.xml"
		BASENAME xdg-shell)
	ecm_add_wayland_client_protocol(WAYLAND_PLATFORM_SRCS
		PROTOCOL "${WAYLAND_PROTOCOLS_PKGDATADIR}/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml"
		BASENAME xdg-decoration)
	add_library(pcsx2-nogui-wayland-protocols STATIC ${WAYLAND_PLATFORM_SRCS})
	target_include_directories(pcsx2-nogui-wayland-protocols PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

	target_sources(pcsx2-nogui PRIVATE
		WaylandNoGUIPlatform.cpp
		WaylandNoGUIPlatform.h
	)
	target_compile_definitions(pcsx2-nogui PRIVATE
		NOGUI_PLATFORM_WAYLAND
	)
	target_link_libraries(pcsx2-nogui PRIVATE
		pcsx2-nogui-wayland-protocols
	 	Wayland::Client
		XKBCommon::XKBCommon
	)
endif()

if (APPLE)
	function(pcsx2_resource path basedir)
	        get_filename_component(dir ${path} DIRECTORY)
	        file(RELATIVE_PATH subdir ${basedir} ${dir})
                target_sources(pcsx2-nogui PRIVATE ${path})
                set_source_files_properties(${path} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/${subdir})
	endfunction()

	set_target_properties(pcsx2-nogui PROPERTIES
		MACOSX_BUNDLE true
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
		OUTPUT_NAME PCSX2
	)

	file(GLOB_RECURSE RESOURCE_FILES ${CMAKE_SOURCE_DIR}/bin/resources/*)
	foreach(path IN LISTS RESOURCE_FILES)
	        get_filename_component(file ${path} NAME)
	        if("${file}" MATCHES "^\\.") # Don't copy macOS garbage (mainly Finder's .DS_Store files) into application
	                continue()
	        endif()
	        if (NOT WIN32 AND "${path}" MATCHES "/dx11/") # Don't include unneccessary stuff
	                continue()
	        endif()
	        if ("${path}" MATCHES "/locale/") # Generate locales with gettext instead of copying them from bin
	                continue()
	        endif()
	        pcsx2_resource(${path} ${CMAKE_SOURCE_DIR}/bin/resources/)
		message("res ${path}")
	endforeach()

	# Import metallib files.
	add_custom_command(TARGET pcsx2-nogui POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different $<TARGET_FILE_DIR:PCSX2>/*.metallib "$<TARGET_FILE_DIR:pcsx2-nogui>/../Resources/"
	)

	# Icon file.
	pcsx2_resource(${CMAKE_CURRENT_SOURCE_DIR}/PCSX2.icns ${CMAKE_CURRENT_SOURCE_DIR})

	# If they say to skip postprocess bundle, leave the target in but make it so they have
	# to manually run it
	if (SKIP_POSTPROCESS_BUNDLE)
		set(postprocessBundleType "")
	else()
		set(postprocessBundleType ALL)

		add_custom_target(pcsx2-postprocess-bundle ${postprocessBundleType}
			COMMAND ${CMAKE_COMMAND} "-DPCSX2_BUNDLE_PATH=$<TARGET_FILE_DIR:pcsx2-nogui>/../.."
				-P ${CMAKE_SOURCE_DIR}/cmake/Pcsx2PostprocessBundle.cmake
		)
		add_dependencies(pcsx2-postprocess-bundle pcsx2-nogui)
	endif()
endif()
