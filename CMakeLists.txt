project(CBEnchanted CXX C)
cmake_minimum_required(VERSION 2.8)

set(EXE_NAME "CBEnchanted")
set(CMAKE_CXX_FLAGS "-std=c++0x")

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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wno-int-to-pointer-cast -fpermissive -Ofast")
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

# Libraries

#find_package(SFML 2 COMPONENTS graphics system window audio REQUIRED)
#include_directories(${SFML_INCLUDE_DIR})
#list(APPEND LIBS ${SFML_LIBRARIES})

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

list(APPEND LIBS "png")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
list(APPEND LIBS ${OPENGL_LIBRARIES})

if(WIN32)
	set(Boost_USE_STATIC_LIBS   ON)
endif(WIN32)
find_package(Boost COMPONENTS locale REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND LIBS ${Boost_LIBRARIES})

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

# Sources

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)

add_executable(${EXE_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${EXE_NAME} ${LIBS})

