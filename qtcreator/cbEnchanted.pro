QT -= core gui #No qt

TEMPLATE = app

CONFIG += thread precompiled_header

QMAKE_CXXFLAGS_RELEASE += /Zi /nologo /W3 /WX- /MP /Ox /Ob2 /Oi /Ot /Oy- /GL /Gm /EHsc /MD /GS /Gy /fp:fast /Zc:wchar_t /Zc:forScope

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
    ../src/drawingprimitives.h \
    ../src/cbvariableholder.h \
    ../src/cbenchanted.h \
    ../src/camerainterface.h \
    ../src/animinterface.h

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
    ../src/drawingprimitives.cpp \
    ../src/cbvariableholder.cpp \
    ../src/cbenchanted.cpp \
    ../src/camerainterface.cpp \
    ../src/animinterface.cpp


PRECOMPILED_HEADER = ../src/precomp.h
DEFINES += SFML_STATIC

win32{
    CONFIG(release, debug|release){
        LIBS += -L"$$(SFML_LIB)/Release" -lsfml-main -lsfml-window-s -lsfml-audio-s -lsfml-graphics-s -lsfml-system-s
        PRE_TARGETDEPS += "$$(SFML_LIB)/release/sfml-main.lib" "$$(SFML_LIB)/release/sfml-window-s.lib" "$$(SFML_LIB)/release/sfml-audio-s.lib" "$$(SFML_LIB)/release/sfml-graphics-s.lib"
    }
    else {
        LIBS += -L"$$(SFML_LIB)/Debug" -lsfml-main-d -lsfml-window-s-d -lsfml-audio-s-d -lsfml-graphics-s-d -lsfml-system-s-d
        PRE_TARGETDEPS += "$$(SFML_LIB)/debug/sfml-main-d.lib" "$$(SFML_LIB)/debug/sfml-window-s-d.lib" "$$(SFML_LIB)/debug/sfml-audio-s-d.lib" "$$(SFML_LIB)/debug/sfml-graphics-s-d.lib"
    }


    LIBS += -lkernel32 -luser32 #-lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
    DEFINES += WIN32
    INCLUDEPATH += "$$(SFML_INCLUDE)"
    DEPENDPATH += "$$(SFML_INCLUDE)"
    INCLUDEPATH += "$$(BOOST_INCLUDE)"
    DEPENDPATH += "$$(BOOST_INCLUDE)"

}