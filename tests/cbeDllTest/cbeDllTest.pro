QT -= core gui
CONFIG -= qt
CONFIG += dll
TEMPLATE = lib

win32 {
	DEFINES += _CRT_SECURE_NO_WARNINGS
	QMAKE_CXXFLAGS_DEBUG = -Zc:wchar_t -Zc:forScope -MP -W3 -EHsc -MDd -Zi
	QMAKE_CXXFLAGS_RELEASE = -Zc:wchar_t -Zc:forScope -MP -W3 -MD -EHsc -GL -Ox

	# Add some flags to CFLAGS too
	QMAKE_CFLAGS_DEBUG = -MDd -Zc:wchar_t -Zi
	QMAKE_CFLAGS_RELEASE = -MD -Zc:wchar_t -Ox
	# Support only Win2K and above
	DEFINES += _WIN32_WINNT=0x0500
	CONFIG(release, debug|release){
			LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.10-static-md -lallegro_ttf-5.0.10-static-md -lallegro_primitives-5.0.10-static-md -lallegro_image-5.0.10-static-md -lallegro_memfile-5.0.10-static-md -lallegro_font-5.0.10-static-md -lallegro_dialog-5.0.10-static-md -lallegro_audio-5.0.10-static-md -lallegro_acodec-5.0.10-static-md
			LIBS += -llibvorbisfile-1.3.2-static-md -lopenal-1.14-static-md -lzlib-1.2.5-static-md -llibogg-1.2.1-static-md -llibFLAC-1.2.1-static-md -lfreetype-2.4.8-static-md -ldumb-0.9.3-static-md -llibvorbis-1.3.2-static-md
		LIBS += -L"../../cbEnchanted-libbuild-release/release" -lcbEnchanted
		DEFINES += NDEBUG
	}
	else {
			LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.10-static-md-debug -lallegro_ttf-5.0.10-static-md-debug -lallegro_primitives-5.0.10-static-md-debug -lallegro_image-5.0.10-static-md-debug -lallegro_memfile-5.0.10-static-md-debug -lallegro_font-5.0.10-static-md-debug -lallegro_dialog-5.0.10-static-md-debug -lallegro_audio-5.0.10-static-md-debug -lallegro_acodec-5.0.10-static-md-debug
			LIBS += -llibvorbisfile-1.3.2-static-md-debug -lopenal-1.14-static-md-debug -lzlib-1.2.5-static-md-debug -llibogg-1.2.1-static-md-debug -llibFLAC-1.2.1-static-md-debug -lfreetype-2.4.8-static-md-debug -ldumb-0.9.3-static-md-debug -llibvorbis-1.3.2-static-md-debug
		LIBS += -L"../../cbEnchanted-libbuild-debug/debug" -lcbEnchanted
	}
	LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lopengl32 -lPsapi -lWinmm -lShlwapi -lgdiplus

	INCLUDEPATH += "$$(BOOST_INCLUDE)" "$$(ALLEGRO_INCLUDE)" "../../src"
	DEFINES += ALLEGRO_NO_MAGIC_MAIN
}

SOURCES += \
    main.cpp
