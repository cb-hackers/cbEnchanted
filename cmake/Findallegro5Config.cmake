# - Try to find allegro5
# Once done this will define
#
#  ALLEGRO5_FOUND - system has allegro5
#  ALLEGRO5_INCLUDE_DIR - the allegro5 include directory
#  ALLEGRO5_LIBRARIES - Link these to use allegro5
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

set(ALLEGRO_INCLUDE $ENV{ALLEGRO_INCLUDE} CACHE STRING "Allegro 5 include directory path")
set(ALLEGRO_LIB $ENV{ALLEGRO_LIB} CACHE STRING "Allegro 5 library directory path")

if ( ALLEGRO5_INCLUDE_DIR AND ALLEGRO5_LIBRARIES )
   # in cache already
   SET(allegro5_FIND_QUIETLY TRUE)
endif ( ALLEGRO5_INCLUDE_DIR AND ALLEGRO5_LIBRARIES )

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
if( NOT WIN32 )
  find_package(PkgConfig)

  pkg_check_modules(ALLEGRO5_PKG QUIET ALLEGRO5)
endif( NOT WIN32 )

FIND_PATH(ALLEGRO5_INCLUDE_DIR NAMES allegro5/allegro.h
  PATHS
    /usr/local/include
    /usr/X11/include
    /usr/include
	${CMAKE_BINARY_DIR}/packages/Allegro/build/native/include
	${OSX_TOOLCHAIN_PKGS}/include
	${ALLEGRO_INCLUDE}
  HINTS
    ${ALLEGRO5_PKG_INCLUDE_DIRS} # Generated by pkg-config
)

FIND_LIBRARY(ALLEGRO5_LIBRARIES NAMES allegro ${ALLEGRO5_PKG_LIBRARIES}
  PATHS
    /usr/local
    /usr/X11
    /usr
	${CMAKE_BINARY_DIR}/packages/Allegro/build/native/v140/x64
	${CMAKE_BINARY_DIR}/packages/Allegro/build/native/v140/win32
	${CMAKE_BINARY_DIR}/packages/Allegro/build/native/v141/x64
	${CMAKE_BINARY_DIR}/packages/Allegro/build/native/v141/win32
	${OSX_TOOLCHAIN_PKGS}
	${ALLEGRO_LIB}
  HINTS
    ${ALLEGRO5_PKG_LIBRARY_DIRS} # Generated by pkg-config
  PATH_SUFFIXES
    lib64
    lib
)

get_filename_component(ALLEGRO5_LIBRARY_DIR ${ALLEGRO5_LIBRARIES} DIRECTORY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALLEGRO5 DEFAULT_MSG ALLEGRO5_LIBRARIES ALLEGRO5_INCLUDE_DIR )

# show the ALLEGRO5_INCLUDE_DIR and ALLEGRO5_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED(ALLEGRO5_INCLUDE_DIR ALLEGRO5_LIBRARIES )
