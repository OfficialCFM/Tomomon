#ifndef _kpFont_h_
#define _kpFont_h_

#include "kpCommon.h"
#include "kpError.h"
#include "kpBitmap.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#define MAX_CHARS 128

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
 * A character is a glyph set to a char type
 */
struct kpCharacter
{
	char key;
	struct kpGlyph glyph;
};

/*
 * The actual font itself
 */
struct kpFont
{
	struct kpCharacter fontCollection[MAX_CHARS];
	FT_Library ftLib;
	FT_Face ftFace;
};

struct kpFont *kpLoadFont(char *path, uint32_t size);
void kpDrawFontString(struct kpBitmap *dest, struct kpFont *font, char *msg, int32_t xo, int32_t yo);
void kpDestroyFont(struct kpFont *font);

#endif /* _kpFont_h_ */
