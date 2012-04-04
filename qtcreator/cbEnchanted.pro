QT -= core gui #No qt

TEMPLATE = app

CONFIG += thread console


QMAKE_CXXFLAGS_DEBUG += /Zc:wchar_t /Zi


contains(CBE_CONFIG,optimized_debug) {
	DEFINES += LOG_LEVEL_DEBUG LOG_LEVEL_INFO LOG_LEVEL_FIXME
	QMAKE_CXXFLAGS_DEBUG += /Zi /Zc:wchar_t /nologo /W3 /WX- /MP /Ox /Ob2 /Oi /Ot /Oy- /GL /EHsc /GS /Gy /Zc:forScope
} else {
	!contains(CBE_CONFIG,full_optimization) {
		CONFIG += precompiled_header
		DEFINES += LOG_LEVEL_DEBUG LOG_LEVEL_INFO LOG_LEVEL_FIXME
	}
	QMAKE_CXXFLAGS_RELEASE += /nologo /W3 /WX- /MP /Ox /Ob2 /Oi /Ot /Oy- /GL /EHsc /GS /Gy /Zc:forScope /Zc:wchar_t
}




HEADERS += \
    ../src/textinterface.h \
    ../src/sysinterface.h \
    ../src/stringinterface.h \
    ../src/soundinterface.h \
    ../src/precomp.h \
    ../src/objectinterface.h \
    ../src/meminterface.h \
    ../src/mathoperations.h \
    ../src/mathinterface.h \
    ../src/mapinterface.h \
    ../src/inputinterface.h \
    ../src/imageinterface.h \
    ../src/gfxinterface.h \
    ../src/fileinterface.h \
    ../src/effectinterface.h \
    ../src/cbvariableholder.h \
    ../src/cbenchanted.h \
    ../src/camerainterface.h \
    ../src/animinterface.h \
    ../src/any.h \
	../src/debug.h \
    ../src/camerainterface.h \
    ../src/cbobject.h \
    ../src/cbimage.h \
    ../src/cbmap.h \
    ../src/collisioncheck.h \
    ../src/collision.h \
    ../src/util.h \
    ../src/cbparticleemitter.h \
    ../src/particle.h \
    ../src/isstring.h \
    ../src/rendertarget.h \
    ../src/customfunctionhandler.h \
    ../src/customfunction.h

SOURCES += \
    ../src/textinterface.cpp \
    ../src/sysinterface.cpp \
    ../src/stringinterface.cpp \
    ../src/soundinterface.cpp \
    ../src/objectinterface.cpp \
    ../src/meminterface.cpp \
    ../src/mathoperations.cpp \
    ../src/mathinterface.cpp \
    ../src/mapinterface.cpp \
    ../src/main.cpp \
    ../src/inputinterface.cpp \
    ../src/imageinterface.cpp \
    ../src/gfxinterface.cpp \
    ../src/fileinterface.cpp \
    ../src/effectinterface.cpp \
    ../src/cbvariableholder.cpp \
    ../src/cbenchanted.cpp \
    ../src/camerainterface.cpp \
    ../src/animinterface.cpp \
    ../src/camerainterface.cpp \
    ../src/cbobject.cpp \
    ../src/cbimage.cpp \
    ../src/cbmap.cpp \
	../src/collisioncheck.cpp \
    ../src/cbparticleemitter.cpp \
    ../src/debug.cpp \
    ../src/rendertarget.cpp \
    ../src/any.cpp \
    ../src/customfunctionhandler.cpp \
    ../src/customfunction.cpp


PRECOMPILED_HEADER = ../src/precomp.h
DEFINES += ALLEGRO_STATICLINK


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


