QT -= core gui #No qt

TEMPLATE = app

CONFIG += thread console

# Remove some automatically generated compiler flags
QMAKE_CFLAGS = -nologo
QMAKE_CXXFLAGS = -nologo
QMAKE_CFLAGS_WARN_ON =
QMAKE_CXXFLAGS_WARN_ON =
QMAKE_CFLAGS_RELEASE =
QMAKE_CXXFLAGS_RELEASE =
QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO =
QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO =
QMAKE_CFLAGS_DEBUG =
QMAKE_CXXFLAGS_DEBUG =
QMAKE_CFLAGS_LTCG =
QMAKE_CXXFLAGS_LTCG =
QMAKE_CFLAGS_MP =
QMAKE_CXXFLAGS_MP =

# Compiler flags and their meanings in order of appending
#  - Make wchar_t a native type
#  - Use standard scope behavior with for-loops
#  - Compile multiple source files by using multiple processes
#  - Set warning level to 3
#  - Catch C++ exceptions only and tell the compiler to assume that extern C functions never throw a C++ exception.
#  - Create single threaded executable (with and without debug)
# Debug only:
#  - Generate complete debugging information
# Release only:
#  - Enable whole program optimization
#  - Use maximum optimization (/Ob2gity /Gs)
QMAKE_CXXFLAGS_DEBUG = -Zc:wchar_t -Zc:forScope -MP -W3 -EHsc -MTd -Zi
QMAKE_CXXFLAGS_RELEASE = -Zc:wchar_t -Zc:forScope -MP -W3 -MT -EHsc -GL -Ox

# Add some flags to CFLAGS too
QMAKE_CFLAGS_DEBUG += -MTd -Zc:wchar_t -Zi
QMAKE_CFLAGS_RELEASE += -MT -Zc:wchar_t -Ox

# Disable security warnings
DEFINES += _CRT_SECURE_NO_WARNINGS

contains(CBE_CONFIG,optimized_debug) {
	DEFINES += LOG_LEVEL_DEBUG LOG_LEVEL_INFO LOG_LEVEL_FIXME LOG_LEVEL_STUB
	#DEFINES += VC_USE_FORCEINLINE
	QMAKE_LFLAGS_RELEASE += -DEBUG
	QMAKE_CXXFLAGS_RELEASE += -Zi
	QMAKE_CFLAGS_RELEASE += -Zi
} else {
	!contains(CBE_CONFIG,full_optimization) {
		CONFIG += precompiled_header
		DEFINES += LOG_LEVEL_DEBUG LOG_LEVEL_INFO LOG_LEVEL_FIXME LOG_LEVEL_STUB
	}
	# Compiler flags:
	#  - Enable whole program optimization
	#  - Catch C++ exceptions only and tell the compiler to assume that
	#    extern C functions never throw a C++ exception
	QMAKE_CXXFLAGS_RELEASE += -GL -EHsc
	DEFINES += VC_USE_FORCEINLINE
}

contains(CBE_CONFIG,debug_hc) {
	DEFINES += LOG_LEVEL_HCDEBUG
}



HEADERS += ../src/*.h

SOURCES += ../src/*.cpp


PRECOMPILED_HEADER = ../src/precomp.h
DEFINES += ALLEGRO_STATICLINK

!contains(CBE_CONFIG,no_testing) {
	DEFINES += TEST
}

win32{
	# Support only Win2K and above
	DEFINES += _WIN32_WINNT=0x0500
	CONFIG(release, debug|release){
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.6-monolith-static-mt -llibvorbisfile-1.3.2-static-mt -lzlib-1.2.5-static-mt -llibogg-1.2.1-static-mt -llibFLAC-1.2.1-static-mt -lfreetype-2.4.8-static-mt -ldumb-0.9.3-static-mt -llibvorbis-1.3.2-static-mt
		DEFINES += NDEBUG
	}
	else {
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.6-monolith-static-mt-debug -llibvorbisfile-1.3.2-static-mt-debug -lzlib-1.2.5-static-mt-debug -llibogg-1.2.1-static-mt-debug -llibFLAC-1.2.1-static-mt-debug -lfreetype-2.4.8-static-mt-debug -ldumb-0.9.3-static-mt-debug -llibvorbis-1.3.2-static-mt-debug
	}
	LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lopengl32 -lPsapi -lWinmm -lShlwapi -lgdiplus
	INCLUDEPATH += "$$(BOOST_INCLUDE)" "$$(ALLEGRO_INCLUDE)"
}
