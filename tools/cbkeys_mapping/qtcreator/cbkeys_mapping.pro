TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += ..\\main.cpp


win32 {
	LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lopengl32 -lPsapi -lWinmm -lShlwapi

	DEFINES += WIN32 ALLEGRO_STATICLINK
	INCLUDEPATH += "$$(ALLEGRO_INCLUDE)"
	DEPENDPATH += "$$(ALLEGRO_INCLUDE)"
	CONFIG(release, debug|release){
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.6-monolith-static-md -llibvorbisfile-1.3.2-static-md -lzlib-1.2.5-static-md -llibogg-1.2.1-static-md -llibFLAC-1.2.1-static-md -lfreetype-2.4.8-static-md -ldumb-0.9.3-static-md -llibvorbis-1.3.2-static-md
		PRE_TARGETDEPS += "$$(ALLEGRO_LIB)/allegro-5.0.6-monolith-static-md.lib"
	}
	else {
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.6-monolith-static-md-debug -llibvorbisfile-1.3.2-static-md-debug -lzlib-1.2.5-static-md-debug -llibogg-1.2.1-static-md-debug -llibFLAC-1.2.1-static-md-debug -lfreetype-2.4.8-static-md-debug -ldumb-0.9.3-static-md-debug -llibvorbis-1.3.2-static-md-debug
		PRE_TARGETDEPS += "$$(ALLEGRO_LIB)/allegro-5.0.6-monolith-static-md-debug.lib"
	}
}

