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
const unordered_map<string,string> getListOfNormalFonts() {
	unordered_map<string,string> fonts;
	fonts.rehash(62);

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

string findfont(const char* font, bool isBold, bool isItalic) {
	static string fontdir = getenv("WINDIR") + string("\\Fonts\\");
	static unordered_map<string,string> normalfonts = getListOfNormalFonts();

	string fontname = font;
	transform(fontname.begin(), fontname.end(), fontname.begin(), tolower);

	auto it = normalfonts.find(fontname);

	if (it != normalfonts.end()) {
		return fontdir + it->second;
	}
	else {
		// Font not found, let the font loader know it.
		return "";
	}
}

#endif // _WIN32
