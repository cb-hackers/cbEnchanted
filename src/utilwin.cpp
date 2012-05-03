#ifdef _WIN32

#include "util.h"
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>

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
const unordered_map<string,string> getMapOfNormalFonts() {
	unordered_map<string,string> fonts;

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
const unordered_map<string,string> getMapOfBoldFonts() {
	unordered_map<string,string> fonts;

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
const unordered_map<string,string> getMapOfItalicFonts() {
	unordered_map<string,string> fonts;

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
const unordered_map<string,string> getMapOfBoldItalicFonts() {
	unordered_map<string,string> fonts;

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

string findfont(const char* font, bool isBold, bool isItalic) {
	static string fontdir = getenv("WINDIR") + string("\\Fonts\\");
	static unordered_map<string,string> normalfonts = getMapOfNormalFonts();
	static unordered_map<string,string> boldfonts = getMapOfBoldFonts();
	static unordered_map<string,string> italicfonts = getMapOfItalicFonts();
	static unordered_map<string,string> bolditalicfonts = getMapOfBoldItalicFonts();

	// Lowercase the wanted font
	string fontname = font;
	transform(fontname.begin(), fontname.end(), fontname.begin(), tolower);

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

#endif // _WIN32
