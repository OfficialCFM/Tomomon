#ifndef _kpGUI_h_
#define _kpGUI_h_

#include "kpCommon.h"
#include "kpError.h"
#include "kpBitmap.h"

#include "ft2build.h"
#include FT_FREETYPE_H

extern struct kpBitmap *panelSheet;

#define MAX_CHARS 128
#define GUI_WIDTH 8

/*
 * All of the information needed for drawing a character
 */
struct kpGlyph
{
	struct kpVec2i size, bearing;
	struct kpBitmap *bitmap;
	int64_t advance;
};

/*
 * The actual font itself
 */
struct kpFont
{
	struct kpGlyph glyphs[128];
	FT_Library ftlib;
	FT_Face ftface;
};

struct kpFont *kpLoadFont(char *path, uint32_t size);
void kpDrawFontString(struct kpBitmap *dest, struct kpFont *font, char *msg, int32_t xo, int32_t yo);
void kpDestroyFont(struct kpFont *font);

/*
 * A simple panel
 */
struct kpPanel
{
	int32_t x, y;
	uint32_t w, h;
};

/* Probably the biggest clusterfuck of code in here so far */
void kpDrawPanel(struct kpBitmap *dest, struct kpPanel *src);

#endif /* _kpGUI_h_ */
