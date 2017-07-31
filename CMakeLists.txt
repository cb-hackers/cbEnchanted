project(cbEnchanted CXX C)
cmake_minimum_required(VERSION 3.1)

set(EXE_NAME "cbEnchanted")
set(CMAKE_CXX_STANDARD 11)

if(CROSS_STATIC_MINGW)
	set(EXE_NAME "cbEnchanted.exe")

	INCLUDE(CMakeForceCompiler)

	SET(CMAKE_SYSTEM_NAME Windows)
	SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
	SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
	SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
	SET(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

	set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
	
	SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
	
	add_definitions(-DALLEGRO_STATICLINK)
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	SET(BUILD_SHARED_LIBRARIES OFF)
	
	if(DEFINED ENV{ALLEGRO_INCLUDE})
		include_directories($ENV{ALLEGRO_INCLUDE})
	else()
		message(FATAL_ERROR "ENV{ALLEGRO_INCLUDE} is not set.")
	endif()
	if(DEFINED ENV{ALLEGRO_LIB})
		link_directories($ENV{ALLEGRO_LIB})
	else()
		message(FATAL_ERROR "ENV{ALLEGRO_LIB} is not set.")
	endif()
	if(DEFINED ENV{ALLEGRO_DEPS_LIB})
		link_directories($ENV{ALLEGRO_DEPS_LIB})
	else()
		message(FATAL_ERROR "ENV{ALLEGRO_DEPS_LIB} is not set.")
	endif()
	if(DEFINED ENV{BOOST_INCLUDE})
		include_directories($ENV{BOOST_INCLUDE})
	else()
		message(FATAL_ERROR "ENV{BOOST_INCLUDE} is not set.")
	endif()
	if(DEFINED ENV{BOOST_LIB})
		link_directories($ENV{BOOST_LIB})
	else()
		message(FATAL_ERROR "ENV{BOOST_LIB} is not set.")
	endif()	
endif(CROSS_STATIC_MINGW)

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
endif()

add_definitions(-DLOG_LEVEL_STUB -DLOG_LEVEL_FIXME -DLOG_LEVEL_DEBUG -DLOG_LEVEL_INFO)

# If we want to build a testable CBEnchanted binary, we use flag -D TEST:bool=true
if(TEST)
	add_definitions(-DTEST)
	message(STATUS "Added definition TEST")
else()
	set(TEST false CACHE STRING "Whether creating testable binary or not, boolean value.")
endif()

# Include additional CMake scripts
#include(/usr/share/cmake-2.8/Modules/FindBoost.cmake)
include(cmake/FindIConv.cmake)
include(cmake/FindFontConfig.cmake)

# Include path for additional CMake library finding scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# We want the binaries to be easily accessible
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if(NOT CROSS_STATIC_MINGW)
	# Libraries
	#find_package(allegro REQUIRED)
	include_directories("/usr/include/allegro5")
	list(APPEND LIBS "allegro")
	list(APPEND LIBS "allegro_font")
	list(APPEND LIBS "allegro_ttf")
	list(APPEND LIBS "allegro_primitives")
	list(APPEND LIBS "allegro_audio")
	list(APPEND LIBS "allegro_acodec")
	list(APPEND LIBS "allegro_image")
	list(APPEND LIBS "allegro_dialog")
	list(APPEND LIBS "allegro_memfile")

	list(APPEND LIBS "png")
	list(APPEND LIBS "openal")
	if(UNIX)
		list(APPEND LIBS "dl")
	endif()
endif(NOT CROSS_STATIC_MINGW)

if(WIN32)
	set(Boost_USE_STATIC_LIBS   ON)
endif(WIN32)

if(FONTCONFIG_FOUND)
	message(STATUS "Font config found at ${FONTCONFIG_INCLUDE_DIR}")
	include_directories("${FONTCONFIG_INCLUDE_DIR}/fontconfig/")
	list(APPEND LIBS ${FONTCONFIG_LIBRARIES})
	add_definitions(-DFONTCONFIG_FOUND)
endif()

if(NOT CROSS_STATIC_MINGW)
	find_package(Boost COMPONENTS locale REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
	list(APPEND LIBS ${Boost_LIBRARIES})

	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIR})
	list(APPEND LIBS ${OPENGL_LIBRARIES})

	if(ICONV_FOUND)
		message(STATUS "Found iconv")
		include_directories("${ICONV_INCLUDE_DIR}")
		list(APPEND LIBS ${ICONV_LIBRARIES})
	else()
		message(FATAL_ERROR "iconv was not found, please install it or check this cmake script!")
	endif()
endif(NOT CROSS_STATIC_MINGW)

# Sources

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)

add_executable(${EXE_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${EXE_NAME} ${LIBS})

if(CROSS_STATIC_MINGW)
	TARGET_LINK_LIBRARIES(${EXE_NAME}
		allegro_acodec-static
		allegro_audio-static
		allegro_dialog-static
		allegro_font-static
		allegro_memfile-static
		allegro_image-static
		allegro_primitives-static
		allegro_ttf-static
		allegro-static
		jpeg
		png16
		zlib
		freetype
		FLAC
		dumb
		vorbisfile
		vorbis
		ogg
		dsound
		opusfile
		opus
		theoradec
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

	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif(CROSS_STATIC_MINGW)