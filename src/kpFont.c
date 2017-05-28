#include "kpFont.h"

struct kpFont *kpLoadFont(char *path, uint32_t size)
{
	uint32_t i, j;

	struct kpFont *font = ALLOC(struct kpFont);
	font->ftLib = NULL;
	font->ftFace = NULL;

	if (FT_Init_FreeType(&font->ftLib))
		kpLogError(kpParseError(KP_FONT_FREETYPE_FAILURE));

	if (FT_New_Face(font->ftLib, path, 0, &font->ftFace))
		kpLogError("%s: \"%s\"", kpParseError(KP_FILE_NOT_FOUND), path);

	if (FT_Set_Pixel_Sizes(font->ftFace, 0, size))
		kpLogError(kpParseError(KP_FONT_DIMENSIONS_FAILURE));

	for (i = 0; i < MAX_CHARS; ++i)
	{
		if (FT_Load_Char(font->ftFace, i, FT_LOAD_RENDER | FT_LOAD_NO_HINTING))
		{
			kpLogError(kpParseError(KP_FONT_FAILED_TO_LOAD_CHAR));
			continue;
		}

		int64_t advance = font->ftFace->glyph->advance.x;
		uint32_t width = font->ftFace->glyph->bitmap.width;
		uint32_t height = font->ftFace->glyph->bitmap.rows;
		int32_t left = font->ftFace->glyph->bitmap_left;
		int32_t top = font->ftFace->glyph->bitmap_top;
		uint32_t buffer[width * height];

		for (j = 0; j < width * height; ++j)
		{
			uint8_t out = font->ftFace->glyph->bitmap.buffer[j];
			buffer[j] = (out == 255 ? out : 0) << 24;
		}

		struct kpBitmap *bitmap = kpCreateBitmap(width, height);
		memcpy(bitmap->px, buffer, sizeof buffer);

		font->fontCollection[i].key = i;
		font->fontCollection[i].glyph.size.x = width;
		font->fontCollection[i].glyph.size.y = height;
		font->fontCollection[i].glyph.bearing.x = left;
		font->fontCollection[i].glyph.bearing.y = top;
		font->fontCollection[i].glyph.bitmap = bitmap;
		font->fontCollection[i].glyph.advance = advance;
	}

	return font;
}

void kpDrawFontString(struct kpBitmap *dest, struct kpFont *font, char *msg, int32_t xo, int32_t yo)
{
	uint8_t i;
	int32_t ox;

	ox = xo;

	for (i = 0; i < strlen(msg); ++i)
	{
		struct kpGlyph g = font->fontCollection[(uint8_t)(msg[i])].glyph;
		struct kpGlyph h = font->fontCollection['H'].glyph;

		if (msg[i] == '\n')
		{
			yo += 14;
			xo = ox;

			continue;
		}

		int xp = xo + g.bearing.x;
		int yp = yo + h.bearing.y - g.bearing.y;

		kpDrawBitmap(dest, g.bitmap, xp, yp);

		xo += g.advance >> 6;
	}
}

void kpDestroyFont(struct kpFont *font)
{
	uint32_t i;

	for (i = 0; i < MAX_CHARS; ++i)
		kpDestroyBitmap(font->fontCollection[i].glyph.bitmap);

	FT_Done_Face(font->ftFace);
	FT_Done_FreeType(font->ftLib);

	FREE(font);
}
