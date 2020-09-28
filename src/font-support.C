#include <stdint.h>
#include <string>
#include <fontconfig/fontconfig.h>
#include "font-support.H"

std::string findFont(std::string name) {
	std::string result = "";
	FcInit();
	FcConfig *c = FcInitLoadConfigAndFonts();
	FcPattern *p = FcNameParse((FcChar8 *)name.c_str());
	FcConfigSubstitute(c, p, FcMatchPattern);
	FcDefaultSubstitute(p);
	FcResult r;
	FcPattern *f = FcFontMatch(c, p, &r);
	if(f) {
		FcChar8 *fn;
		if(FcPatternGetString(f, FC_FILE, 0, &fn) == FcResultMatch) {
			result = std::string((char *)fn);
		}
	}
	FcPatternDestroy(f);
	FcPatternDestroy(p);
	FcConfigDestroy(c);
	FcFini();

	return result;
}

