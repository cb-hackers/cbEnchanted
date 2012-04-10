QT -= core gui #No qt

TEMPLATE = app

CONFIG += thread console


QMAKE_CXXFLAGS_DEBUG += /Zc:wchar_t /Zi


contains(CBE_CONFIG,optimized_debug) {
	DEFINES += LOG_LEVEL_DEBUG LOG_LEVEL_INFO LOG_LEVEL_FIXME
	DEFINES += VC_USE_FORCEINLINE
	QMAKE_CXXFLAGS_RELEASE += /Zi /Zc:wchar_t /Ox
	QMAKE_LFLAGS_RELEASE += /DEBUG
} else {
	!contains(CBE_CONFIG,full_optimization) {
		CONFIG += precompiled_header
		DEFINES += LOG_LEVEL_DEBUG LOG_LEVEL_INFO LOG_LEVEL_FIXME
	}
	QMAKE_CXXFLAGS_RELEASE += /nologo /W3 /WX- /MP /Ox /Ob2 /Oi /Ot /Oy- /GL /GL /EHsc /Gy /Zc:forScope /Zc:wchar_t
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
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.6-monolith-static-md -llibvorbisfile-1.3.2-static-md -lzlib-1.2.5-static-md -llibogg-1.2.1-static-md -llibFLAC-1.2.1-static-md -lfreetype-2.4.8-static-md -ldumb-0.9.3-static-md -llibvorbis-1.3.2-static-md
		DEFINES += NDEBUG
	}
	else {
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.6-monolith-static-md-debug -llibvorbisfile-1.3.2-static-md-debug -lzlib-1.2.5-static-md-debug -llibogg-1.2.1-static-md-debug -llibFLAC-1.2.1-static-md-debug -lfreetype-2.4.8-static-md-debug -ldumb-0.9.3-static-md-debug -llibvorbis-1.3.2-static-md-debug
	}
	LIBS += -L"$$(BOOST_LIB)"
	LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lopengl32 -lPsapi -lWinmm -lShlwapi -lgdiplus
	INCLUDEPATH += "$$(BOOST_INCLUDE)" "$$(ALLEGRO_INCLUDE)"
	SOURCES += ../src/utilwin.cpp
}
else {
	SOURCES += ../src/utillinux.cpp
}


