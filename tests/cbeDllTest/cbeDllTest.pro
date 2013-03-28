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
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.9-static -lallegro_ttf-5.0.9-static -lallegro_primitives-5.0.9-static -lallegro_image-5.0.9-static -lallegro_memfile-5.0.9-static -lallegro_font-5.0.9-static -lallegro_dialog-5.0.9-static -lallegro_audio-5.0.9-static -lallegro_acodec-5.0.9-static
		LIBS += -llibvorbisfile_static-1.3.3 -lOpenAL32_static-1.1.5.1 -lzlib_static-1.2.7 -llibogg_static-1.3.0 -llibFLAC_static-1.2.1 -lfreetype_static-2.4.11 -ldumb_static-0.9.3 -llibvorbis_static-1.3.3
		LIBS += -L"../../cbEnchanted-libbuild-release/release" -lcbEnchanted
		DEFINES += NDEBUG
	}
	else {
		LIBS += -L"$$(ALLEGRO_LIB)" -lallegro-5.0.9-debug-static -lallegro_ttf-5.0.9-debug-static -lallegro_primitives-5.0.9-debug-static -lallegro_image-5.0.9-debug-static -lallegro_memfile-5.0.9-debug-static -lallegro_font-5.0.9-debug-static -lallegro_dialog-5.0.9-debug-static -lallegro_audio-5.0.9-debug-static -lallegro_acodec-5.0.9-debug-static
		LIBS += -llibvorbisfile_static-1.3.3-debug -lOpenAL32_static-1.1.5.1-debug -lzlib_static-1.2.7-debug -llibogg_static-1.3.0-debug -llibFLAC_static-1.2.1-debug -lfreetype_static-2.4.11-debug -ldumb_static-0.9.3-debug -llibvorbis_static-1.3.3-debug
		LIBS += -L"../../cbEnchanted-libbuild-debug/debug" -lcbEnchanted
	}
	LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lopengl32 -lPsapi -lWinmm -lShlwapi -lgdiplus

	INCLUDEPATH += "$$(BOOST_INCLUDE)" "$$(ALLEGRO_INCLUDE)" "../../src"
	DEFINES += ALLEGRO_NO_MAGIC_MAIN
}

SOURCES += \
    main.cpp
