# - Try to find ALLEGRO5DEPS
# Once done this will define
#
#  ALLEGRO5DEPS_FOUND - system has ALLEGRO5DEPS
#  ALLEGRO5DEPS_LIBRARIES - Link these to use ALLEGRO5DEPS
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

set(ALLEGRODEPS_LIB $ENV{ALLEGRODEPS_LIB} CACHE STRING "Allegro 5 library dependencies directory path")

if ( ALLEGRO5DEPS_LIBRARIES )
   # in cache already
   SET(ALLEGRO5DEPS_FIND_QUIETLY TRUE)
endif ( ALLEGRO5DEPS_LIBRARIES )

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
if( NOT WIN32 )
  find_package(PkgConfig)

  pkg_check_modules(ALLEGRO5DEPS_PKG QUIET ALLEGRO5DEPS)
endif( NOT WIN32 )

if(MSVC)
	set(ALLEGRO_LIBS_DEPS_PNG16 "libpng16")
	if(${MSVC_VERSION} STREQUAL "1900")
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(NUGET_ALLEGRODEPS_LIBS_DIR ${CMAKE_BINARY_DIR}/packages/AllegroDeps/build/native/v140/x64/deps)
		else()
			set(NUGET_ALLEGRODEPS_LIBS_DIR ${CMAKE_BINARY_DIR}/packages/AllegroDeps/build/native/v140/win32/deps)
		endif()
	else()
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(NUGET_ALLEGRODEPS_LIBS_DIR ${CMAKE_BINARY_DIR}/packages/AllegroDeps/build/native/v141/x64/deps)
		else()
			set(NUGET_ALLEGRODEPS_LIBS_DIR ${CMAKE_BINARY_DIR}/packages/AllegroDeps/build/native/v141/win32/deps)
		endif()	
	endif()
else()
	set(ALLEGRO_LIBS_DEPS_PNG16 "png16")
endif()

FIND_LIBRARY(ALLEGRO5DEPS_LIBRARIES NAMES jpeg zlib freetype FLAC dumb vorbisfile vorbis ogg opusfile opus theoradec ${ALLEGRO_LIBS_DEPS_PNG16} ${ALLEGRO5DEPS_PKG_LIBRARIES}
  PATHS
    /usr/local
    /usr/X11
    /usr
	${ALLEGRODEPS_LIB}
	${NUGET_ALLEGRODEPS_LIBS_DIR}
  HINTS
    ${ALLEGRO5DEPS_PKG_LIBRARY_DIRS} # Generated by pkg-config
  PATH_SUFFIXES
    lib64
    lib
)

get_filename_component(ALLEGRO5DEPS_LIBRARY_DIR ${ALLEGRO5DEPS_LIBRARIES} DIRECTORY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Allegro5Deps DEFAULT_MSG ALLEGRO5DEPS_LIBRARIES )

# show the ALLEGRO5DEPS_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED( ALLEGRO5DEPS_LIBRARIES )
