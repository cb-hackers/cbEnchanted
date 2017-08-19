project(cbEnchanted CXX C)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

option(STATIC "Build static executable" OFF)
option(WANT_TEST "Whether creating testable binary or not, boolean value." OFF)
option(WANT_CBCMAKER "Build cbcmaker" OFF)

if(NOT EXE_NAME)
	set(EXE_NAME "cbEnchanted")
endif()

# Avoid source tree pollution
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)

# Add a sensible build type default and warning because empty means no optimization and no debug info.
if(NOT CMAKE_BUILD_TYPE)
	message("WARNING: CMAKE_BUILD_TYPE is not defined!\n         Defaulting to CMAKE_BUILD_TYPE=RelWithDebInfo. Use ccmake to set a proper value.")
	set(CMAKE_BUILD_TYPE RelWithDebInfo
	CACHE STRING "Type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC detected, adding compile flags")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wno-int-to-pointer-cast -fpermissive -Ofast -Wno-format-security")
elseif(MSVC)
	message(STATUS "MSVC detected, adding compile flags")
	set(CXX_FLAGS_ALL "/DWIN32 /D_WINDOWS -nologo -Zc:wchar_t -Zc:forScope -MP -W3 -EHsc")
	set(CMAKE_CXX_FLAGS "${CXX_FLAGS_ALL}")	
	set(CMAKE_CXX_FLAGS_DEBUG "${CXX_FLAGS_ALL} -Zi")	
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CXX_FLAGS_ALL} -GL -Ox")
	set(CMAKE_CXX_FLAGS_RELEASE "${CXX_FLAGS_ALL} -GL -Ox")
	if(STATIC)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MTd")	
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CXX_FLAGS_ALL} -MTd")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MT")
	else()
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MDd")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CXX_FLAGS_ALL} -MDd")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MD")		
	endif()
	add_definitions(-DVC_USE_FORCEINLINE)
endif()

add_definitions(-DLOG_LEVEL_STUB -DLOG_LEVEL_FIXME -DLOG_LEVEL_DEBUG -DLOG_LEVEL_INFO)

# If we want to build a testable CBEnchanted binary, we use flag -D WANT_TEST:bool=true
if(WANT_TEST)
	add_definitions(-DTEST)
	message(STATUS "Added definition TEST")
else()
	set(WANT_TEST OFF CACHE BOOL "Whether creating testable binary or not, boolean value.")
endif()

# Include path for additional CMake library finding scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(ALLEGRO-LIBS-BUILDTYPE-SUFFIX "-debug")
endif()

if(STATIC)
	add_definitions(-DALLEGRO_STATICLINK)
	
	set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
	if (NOT MSVC)
		set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	else()
		set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
	endif()
	set(BUILD_SHARED_LIBRARIES OFF)

	if(MSVC OR MINGW OR CROSS-MINGW)
		set(ALLEGRO-LIBS-STATIC-SUFFIX "-static")
	endif()
endif()

if(MONOLITH_ALLEGRO)
	list(APPEND ALLEGRO_LIBS "allegro_monolith${ALLEGRO-LIBS-STATIC-SUFFIX}")
else()
	list(APPEND ALLEGRO_LIBS
		"allegro_video${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro_acodec${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro_audio${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro_dialog${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro_font${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro_memfile${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro_image${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro_primitives${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro_ttf${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro_main${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
		"allegro${ALLEGRO-LIBS-BUILDTYPE-SUFFIX}${ALLEGRO-LIBS-STATIC-SUFFIX}"
	)
endif()

include(cmake/Findallegro5Config.cmake)

if(ALLEGRO5_FOUND)
	include_directories(${ALLEGRO5_INCLUDE_DIR})
	link_directories(${ALLEGRO5_LIBRARY_DIR})
else()
	message(FATAL_ERROR "Allegro 5 was not found, please install it or check this cmake script!")
endif()

include(cmake/FindEnet.cmake)

if(ENET_FOUND)
	include_directories(${ENET_INCLUDE_DIR})
	link_directories(${ENET_LIBRARY_DIR})
	list(APPEND LIBS enet)
else()
	message(FATAL_ERROR "Enet was not found, please install it or check this cmake script!")
endif()

# Libraries
if(NOT MSVC AND NOT MINGW AND NOT CROSS-MINGW)
	# Include additional CMake scripts
	include(cmake/FindIConv.cmake)
	include(cmake/FindFontConfig.cmake)

	list(APPEND ALLEGRO_LIBS_DEPS 
		png
		dl
	)
	
	if(NOT CROSS-DARWIN AND NOT APPLE)
		list(APPEND ALLEGRO_LIBS_DEPS 
			openal
		)
	endif()

	find_package(Boost COMPONENTS locale REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
	list(APPEND LIBS ${Boost_LIBRARIES})

	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIR})
	list(APPEND LIBS ${OPENGL_LIBRARIES})

	if(FONTCONFIG_FOUND)
		message(STATUS "Font config found at ${FONTCONFIG_INCLUDE_DIR}")
		include_directories("${FONTCONFIG_INCLUDE_DIR}/fontconfig/")
		list(APPEND LIBS ${FONTCONFIG_LIBRARIES})
		add_definitions(-DFONTCONFIG_FOUND)
	endif()
	
	if(ICONV_FOUND)
		message(STATUS "Found iconv")
		include_directories("${ICONV_INCLUDE_DIR}")
		list(APPEND LIBS ${ICONV_LIBRARIES})
	else()
		message(FATAL_ERROR "iconv was not found, please install it or check this cmake script!")
	endif()
else()
	include(cmake/Findallegro5depsConfig.cmake)

	if(ALLEGRO5DEPS_FOUND)
		link_directories(${ALLEGRO5DEPS_LIBRARY_DIR})
	else()
		message(FATAL_ERROR "Dependencies for allegro 5 was not found, please install it or check this cmake script!")
	endif()

	if(DEFINED ENV{BOOST_INCLUDE})
		include_directories($ENV{BOOST_INCLUDE})
	else()
		message(FATAL_ERROR "Environment variable BOOST_INCLUDE is not set.")
	endif()

	if(MSVC)
		list(APPEND ALLEGRO_LIBS_DEPS "libpng16")
	else()
		list(APPEND ALLEGRO_LIBS_DEPS "png16")
	endif()

	list(APPEND ALLEGRO_LIBS_DEPS
		jpeg
		zlib
		freetype
		FLAC
		dumb
		vorbisfile
		vorbis
		ogg
		opusfile
		opus
		theoradec
	)

	list(APPEND OS_LIBS_DEPS
		dsound
		shlwapi
		ws2_32
		advapi32
		ole32
		shell32
		gdi32
		comdlg32
		user32
		glu32
		opengl32
		psapi
		winmm
		kernel32
		uuid
		gdiplus
	)
endif()

# We want the binaries to be easily accessible
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Sources
file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)

add_executable(${EXE_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${EXE_NAME} ${ALLEGRO_LIBS} ${ALLEGRO_LIBS_DEPS} ${OS_LIBS_DEPS} ${LIBS})

if(STATIC AND NOT MSVC AND NOT CROSS-DARWIN)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
elseif(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ltcg" )
endif()

if(WANT_CBCMAKER)
	add_subdirectory(tools)
endif()