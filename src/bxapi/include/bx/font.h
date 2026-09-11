#ifndef BX_FONT_H
#define BX_FONT_H

#include <bx/math.h>

class cFont {
public:
	u8 pad[0x28];
	t2Vector shadowOffset; // 28-30
	t2Vector baseScale; // 38-3c
	t2Vector scale; // 38-3c
	tARGB color; // 40-4c
	tARGB shadowColor; // 50-5c


	void text(float x, float y, const char* pszInput);
	void textf(float x, float y, const char* pszFormat, ...);
};

ML_STATIC_ASSERT(ml_offsetof(cFont, color) == 0x40);
ML_STATIC_ASSERT(ml_offsetof(cFont, shadowColor) == 0x50);

#endif
