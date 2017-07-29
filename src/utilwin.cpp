#ifdef _WIN32

#include "util.h"
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include <unordered_map>
#include <windows.h>
#include "utf8.h"

using namespace std;

int64_t mtimer() {
	return clock() * 1000 / CLOCKS_PER_SEC;
}

float randf() {
	return (float)rand() / RAND_MAX; //TODO: Better generation (Windows RAND_MAX == 0x7fff)
}

int32_t rand(int32_t max) {
	return rand() % (max + 1); //TODO: Better generation (Windows RAND_MAX == 0x7fff)
}

// Returns an unordered_map with keys as lowercase font names
// and their values as font file name.
const unordered_map<string, string> getMapOfNormalFonts() {
	unordered_map<string, string> fonts;
	fonts.rehash(ceil(61 / fonts.max_load_factor()));

	fonts["andalus"] = "andlso.ttf";
	fonts["angsana new"] = "angsa.ttf";
	fonts["angsanaupc"] = "angsau.ttf";
	fonts["arabic transparent"] = "artro.ttf";
	fonts["arial"] = "arial.ttf";
	fonts["arial black"] = "ariblk.ttf";
	fonts["browallia new"] = "browa.ttf";
	fonts["browalliaupc"] = "browau.ttf";
	fonts["comic sans ms"] = "comic.ttf";
	fonts["cordia new"] = "cordia.ttf";
	fonts["cordiaupc"] = "cordiau.ttf";
	fonts["courier new"] = "cour.ttf";
	fonts["david"] = "david.ttf";
	fonts["david transparent"] = "davidtr.ttf";
	fonts["dilleniaupc"] = "upcdl.ttf";
	fonts["estrangelo edessa"] = "estre.ttf";
	fonts["eucrosiaupc"] = "upcel.ttf";
	fonts["fixed miriam transparent"] = "mriamfx.ttf";
	fonts["franklin gothic medium"] = "framd.ttf";
	fonts["frankruehl"] = "frank.ttf";
	fonts["freesiaupc"] = "upcfl.ttf";
	fonts["gautami"] = "gautami.ttf";
	fonts["georgia"] = "georgia.ttf";
	fonts["impact"] = "impact.ttf";
	fonts["irisupc"] = "upcil.ttf";
	fonts["jasmineupc"] = "upcjl.ttf";
	fonts["kodchiangupc"] = "upckl.ttf";
	fonts["latha"] = "latha.ttf";
	fonts["levenim mt"] = "lvnm.ttf";
	fonts["lilyupc"] = "upcll.ttf";
	fonts["lucida console"] = "lucon.ttf";
	fonts["lucida sans unicode"] = "l_10646.ttf";
	fonts["mangal"] = "mangal.ttf";
	fonts["marlett"] = "marlett.ttf";
	fonts["microsoft sans serif"] = "micross.ttf";
	fonts["miriam"] = "mriam.ttf";
	fonts["miriam fixed"] = "mriamc.ttf";
	fonts["miriam transparent"] = "mriamtr.ttf";
	fonts["mv boli"] = "mvboli.ttf";
	fonts["narkisim"] = "nrkis.ttf";
	fonts["palatino linotype"] = "pala.ttf";
	fonts["raavi"] = "raavi.ttf";
	fonts["rod"] = "rod.ttf";
	fonts["rod transparent"] = "rodtr.ttf";
	fonts["shruti"] = "shruti.ttf";
	fonts["simplified arabic"] = "simpo.ttf";
	fonts["simplified arabic fixed"] = "simpfxo.ttf";
	fonts["sylfaen"] = "sylfaen.ttf";
	fonts["symbol"] = "symbol.ttf";
	fonts["tahoma"] = "tahoma.ttf";
	fonts["times new roman"] = "times.ttf";
	fonts["traditional arabic"] = "trado.ttf";
	fonts["trebuchet ms"] = "trebuc.ttf";
	fonts["tunga"] = "tunga.ttf";
	fonts["verdana"] = "verdana.ttf";
	fonts["webdings"] = "webdings.ttf";
	fonts["wingdings"] = "wingding.ttf";
	fonts["simhei"] = "simhei.ttf";
	fonts["fangsong_gb2312"] = "simfang.ttf";
	fonts["dfkai-sb"] = "kaiu.ttf";
	fonts["kaiti_gb2312"] = "simkai.ttf";

	return fonts;
}

// Returns an unordered_map with keys as lowercase font names
// and their values as font file name.
const unordered_map<string, string> getMapOfBoldFonts() {
	unordered_map<string, string> fonts;
	fonts.rehash(ceil(28 / fonts.max_load_factor()));

	fonts["aharoni"] = "ahronbd.ttf";
	fonts["angsana new"] = "angsab.ttf";
	fonts["angsanaupc"] = "angsaub.ttf";
	fonts["arabic transparent"] = "artrbdo.ttf";
	fonts["arial"] = "arialbd.ttf";
	fonts["browallia new"] = "browab.ttf";
	fonts["browalliaupc"] = "browaub.ttf";
	fonts["comic sans ms"] = "comicbd.ttf";
	fonts["cordia new"] = "cordiab.ttf";
	fonts["cordiaupc"] = "cordiaub.ttf";
	fonts["courier new"] = "courbd.ttf";
	fonts["david"] = "davidbd.ttf";
	fonts["dilleniaupc"] = "upcdb.ttf";
	fonts["eucrosiaupc"] = "upceb.ttf";
	fonts["freesiaupc"] = "upcfb.ttf";
	fonts["georgia"] = "georgiab.ttf";
	fonts["irisupc"] = "upcib.ttf";
	fonts["jasmineupc"] = "upcjb.ttf";
	fonts["kodchiangupc"] = "upckb.ttf";
	fonts["levenim mt"] = "lvnmbd.ttf";
	fonts["lilyupc"] = "upclb.ttf";
	fonts["palatino linotype"] = "palab.ttf";
	fonts["simplified arabic"] = "simpbdo.ttf";
	fonts["tahoma"] = "tahomabd.ttf";
	fonts["times new roman"] = "timesbd.ttf";
	fonts["traditional arabic"] = "tradbdo.ttf";
	fonts["trebuchet ms"] = "trebucbd.ttf";
	fonts["verdana"] = "verdanab.ttf";

	return fonts;
}

// Returns an unordered_map with keys as lowercase font names
// and their values as font file name.
const unordered_map<string, string> getMapOfItalicFonts() {
	unordered_map<string, string> fonts;
	fonts.rehash(ceil(21 / fonts.max_load_factor()));

	fonts["angsana new"] = "angsai.ttf";
	fonts["angsanaupc"] = "angsaui.ttf";
	fonts["arial"] = "ariali.ttf";
	fonts["browallia new"] = "browai.ttf";
	fonts["browalliaupc"] = "browaui.ttf";
	fonts["cordia new"] = "cordiai.ttf";
	fonts["cordiaupc"] = "cordiaui.ttf";
	fonts["courier new"] = "couri.ttf";
	fonts["dilleniaupc"] = "upcdi.ttf";
	fonts["eucrosiaupc"] = "upcei.ttf";
	fonts["franklin gothic medium"] = "framdit.ttf";
	fonts["freesiaupc"] = "upcfi.ttf";
	fonts["georgia"] = "georgiai.ttf";
	fonts["irisupc"] = "upcii.ttf";
	fonts["jasmineupc"] = "upcji.ttf";
	fonts["kodchiangupc"] = "upcki.ttf";
	fonts["lilyupc"] = "upcli.ttf";
	fonts["palatino linotype"] = "palai.ttf";
	fonts["times new roman"] = "timesi.ttf";
	fonts["trebuchet ms"] = "trebucit.ttf";
	fonts["verdana"] = "verdanai.ttf";

	return fonts;
}

// Returns an unordered_map with keys as lowercase font names
// and their values as font file name.
const unordered_map<string, string> getMapOfBoldItalicFonts() {
	unordered_map<string, string> fonts;
	fonts.rehash(ceil(20 / fonts.max_load_factor()));

	fonts["angsana new"] = "angsaz.ttf";
	fonts["angsanaupc"] = "angsauz.ttf";
	fonts["arial"] = "arialbi.ttf";
	fonts["browallia new"] = "browaz.ttf";
	fonts["browalliaupc"] = "browauz.ttf";
	fonts["cordia new"] = "cordiaz.ttf";
	fonts["cordiaupc"] = "cordiauz.ttf";
	fonts["courier new"] = "courbi.ttf";
	fonts["dilleniaupc"] = "upcdbi.ttf";
	fonts["eucrosiaupc"] = "upcebi.ttf";
	fonts["freesiaupc"] = "upcfbi.ttf";
	fonts["georgia"] = "georgiaz.ttf";
	fonts["irisupc"] = "upcibi.ttf";
	fonts["jasmineupc"] = "upcjbi.ttf";
	fonts["kodchiangupc"] = "upckbi.ttf";
	fonts["lilyupc"] = "upclbi.ttf";
	fonts["palatino linotype"] = "palabi.ttf";
	fonts["times new roman"] = "timesbi.ttf";
	fonts["trebuchet ms"] = "trebucbi.ttf";
	fonts["verdana"] = "verdanaz.ttf";

	return fonts;
}

string findfont(const char *font, bool isBold, bool isItalic) {
	static string fontdir = getenv("WINDIR") + string("\\Fonts\\");
	static unordered_map<string, string> normalfonts = getMapOfNormalFonts();
	static unordered_map<string, string> boldfonts = getMapOfBoldFonts();
	static unordered_map<string, string> italicfonts = getMapOfItalicFonts();
	static unordered_map<string, string> bolditalicfonts = getMapOfBoldItalicFonts();

	// Lowercase the wanted font
	string fontname = font;
	
    struct make_lower
    {
        int operator()(int ch) const { return std::tolower(ch); }
    };
	transform(fontname.begin(), fontname.end(), fontname.begin(), make_lower());

	if (isBold && isItalic) {
		// Search for bold and italic fonts
		auto it = bolditalicfonts.find(fontname);
		if (it != bolditalicfonts.end()) {
			return fontdir + it->second;
		}
		cerr << "Failed to load bold and italic version of " << font << endl;
		return "";
	}
	if (isBold) {
		// Search for bold fonts
		auto it = boldfonts.find(fontname);
		if (it != boldfonts.end()) {
			return fontdir + it->second;
		}
		cerr << "Failed to load bold version of " << font << endl;
		return "";
	}
	if (isItalic) {
		// Search for italic fonts
		auto it = italicfonts.find(fontname);
		if (it != italicfonts.end()) {
			return fontdir + it->second;
		}
		cerr << "Failed to load bold version of " << font << endl;
		return "";
	}
	// Search for normal fonts
	auto it = normalfonts.find(fontname);
	if (it != normalfonts.end()) {
		return fontdir + it->second;
	}
	cerr << "Failed to load " << font << endl;
	return "";
}


/** Converts UTF-8 to Windows-1252, returns the converted string. */
std::string utf8toCP1252(std::string str) {
	if (str.empty()) {
		return str;
	}
	
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring tmpWstr( size_needed, 0 );
	MultiByteToWideChar (CP_UTF8, 0, &str[0], (int)str.size(), &tmpWstr[0], size_needed);

	size_needed = WideCharToMultiByte(1252, WC_COMPOSITECHECK | WC_DISCARDNS, &tmpWstr[0], (int)tmpWstr.size(), NULL, 0, NULL, NULL);
	std::string retStr(size_needed, 0);
	WideCharToMultiByte(1252, WC_COMPOSITECHECK | WC_DISCARDNS, &tmpWstr[0], (int)tmpWstr.size(), &retStr[0], size_needed, NULL, NULL);

	return retStr;
}

/** Converts Windows-1252 to UTF-8, returns the converted string. */
std::string CP1252toUtf8(std::string str) {
	int size_needed = MultiByteToWideChar(1252, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring tmpWstr(size_needed, 0);
	MultiByteToWideChar(1252, 0, &str[0], (int)str.size(), &tmpWstr[0], size_needed);

	size_needed = WideCharToMultiByte(CP_UTF8, 0, &tmpWstr[0], (int)tmpWstr.size(), NULL, 0, NULL, NULL);
	std::string retStr(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &tmpWstr[0], (int)tmpWstr.size(), &retStr[0], size_needed, NULL, NULL);

	return retStr;
}

/** Returns the given string as a UTF-16 encoded wstring */
std::wstring utf8ToUtf16(std::string str) {
	// Convert str to utf-16 with the amazing UTF8-CPP library
	string::iterator end_it = utf8::find_invalid(str.begin(), str.end());
	vector<uint16_t> utf16;
	utf8::utf8to16(str.begin(), end_it, back_inserter(utf16));

	wstring widestr;
	widestr.resize(utf16.size());
	for (size_t i = 0; i < utf16.size(); i++) {
		widestr[i] = utf16[i];
	}

	return widestr;
}


/** Loads dynamic link library and returns handle to it */
void *loadDLL(const ISString &f) {
	const string &utf8str = f.getUtf8Encoded();
	string::const_iterator end_it = utf8::find_invalid(utf8str.begin(), utf8str.end());
	vector<uint16_t> utf16;
	utf8::utf8to16(utf8str.begin(), end_it, back_inserter(utf16));

	wstring widestr;
	widestr.resize(utf16.size());
	
	for (size_t i = 0; i < utf16.size(); i++) {
		widestr[i] = utf16[i];
	}
	return LoadLibraryW(widestr.c_str());
}

void *getDLLFunction(void *dll, const string &s) {
	return GetProcAddress((HMODULE)dll, s.c_str());
}

#endif // _WIN32
