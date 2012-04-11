QT -= core gui #No qt

TEMPLATE = app

CONFIG += thread console

# Remove some automatically added compiler headers
QMAKE_CFLAGS_DEBUG -= -mtd -nologo -Zm200 -Zc:wchar_t- -Zi
QMAKE_CXXFLAGS_DEBUG -= -mtd -nologo -Zm200 -Zc:wchar_t- -Zi
QMAKE_CFLAGS_RELEASE -= -mt -nologo -Zm200 -Zc:wchar_t- -O2
QMAKE_CXXFLAGS_RELEASE -= -mt -nologo -Zm200 -Zc:wchar_t- -O2

# Compiler flags and their meanings in order of appending
#  - Suppresses display of sign-on banner
#  - Make wchar_t a native type
#  - Use standard scope behavior with for-loops
#  - Compile multiple source files by using multiple processes
#  - Set warning level to 3
#  - Create single threaded executable (with and without debug)
# Debug only:
#  - Generate complete debugging information
# Release only:
#  - Use maximum optimization (/Ob2gity /Gs)
QMAKE_CXXFLAGS_DEBUG += -nologo -Zc:wchar_t -Zc:forScope -MP -W3 -MTd -Zi
QMAKE_CXXFLAGS_RELEASE += -nologo -Zc:wchar_t -Zc:forScope -MP -W3 -MT -Ox

# Add some removed flags to CFLAGS too
QMAKE_CFLAGS_DEBUG += -MTd -nologo -Zc:wchar_t
QMAKE_CFLAGS_RELEASE += -MT -nologo -Zc:wchar_t

contains(CBE_CONFIG,optimized_debug) {
	DEFINES += LOG_LEVEL_DEBUG LOG_LEVEL_INFO LOG_LEVEL_FIXME
	#DEFINES += VC_USE_FORCEINLINE
	QMAKE_LFLAGS_RELEASE += -DEBUG
	QMAKE_CXXFLAGS_RELEASE += -Zi
	QMAKE_CFLAGS_RELEASE += -Zi
} else {
	!contains(CBE_CONFIG,full_optimization) {
		CONFIG += precompiled_header
		DEFINES += LOG_LEVEL_DEBUG LOG_LEVEL_INFO LOG_LEVEL_FIXME
	}
	# Compiler flags:
	#  - Enable whole program optimization
	#  - Catch C++ exceptions only and tell the compiler to assume that
	#    extern C functions never throw a C++ exception
	QMAKE_CXXFLAGS_RELEASE += -GL -EHsc
	DEFINES += VC_USE_FORCEINLINE
}




HEADERS += ../src/*.h \
    ../src/mathoperations.h

SOURCES += ../src/*.cpp


PRECOMPILED_HEADER = ../src/precomp.h
DEFINES += ALLEGRO_STATICLINK

!contains(CBE_CONFIG,no_testing) {
	DEFINES += TEST
}

win32{
	CONFIG(release, debug|release){
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.6-monolith-static-mt -llibvorbisfile-1.3.2-static-mt -lzlib-1.2.5-static-mt -llibogg-1.2.1-static-mt -llibFLAC-1.2.1-static-mt -lfreetype-2.4.8-static-mt -ldumb-0.9.3-static-mt -llibvorbis-1.3.2-static-mt
		DEFINES += NDEBUG
	}
	else {
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.6-monolith-static-mt-debug -llibvorbisfile-1.3.2-static-mt-debug -lzlib-1.2.5-static-mt-debug -llibogg-1.2.1-static-mt-debug -llibFLAC-1.2.1-static-mt-debug -lfreetype-2.4.8-static-mt-debug -ldumb-0.9.3-static-mt-debug -llibvorbis-1.3.2-static-mt-debug
	}
	LIBS += -L"$$(BOOST_LIB)"
	LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lopengl32 -lPsapi -lWinmm -lShlwapi -lgdiplus
	INCLUDEPATH += "$$(BOOST_INCLUDE)" "$$(ALLEGRO_INCLUDE)"
	SOURCES += ../src/utilwin.cpp
}
else {
	SOURCES += ../src/utillinux.cpp
}


