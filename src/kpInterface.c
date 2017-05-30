#include "kpInterface.h"

struct kpBitmap *panelSheet = NULL;

/*
 * The way fonts are handled here is a bit of a hack, but whatever
 */

struct kpFont *kpLoadFont(char *path, uint32_t size)
{
	struct kpFont *font = ALLOC(struct kpFont);
	font->ftlib = NULL;
	font->ftface = NULL;

	if (FT_Init_FreeType(&font->ftlib))
		kpLogError(kpParseError(KP_FONT_FREETYPE_FAILURE));

	if (FT_New_Face(font->ftlib, path, 0, &font->ftface))
		kpLogError("%s: \"%s\"", kpParseError(KP_FILE_NOT_FOUND), path);

	if (FT_Set_Pixel_Sizes(font->ftface, 0, size))
		kpLogError(kpParseError(KP_FONT_DIMENSIONS_FAILURE));

	for (uint32_t i = 0; i < MAX_CHARS; ++i)
	{
		if (FT_Load_Char(font->ftface, i, FT_LOAD_RENDER | FT_LOAD_NO_HINTING))
		{
			kpLogError(kpParseError(KP_FONT_FAILED_TO_LOAD_CHAR));
			continue;
		}

		int64_t advance = font->ftface->glyph->advance.x;
		uint32_t width = font->ftface->glyph->bitmap.width;
		uint32_t height = font->ftface->glyph->bitmap.rows;
		int32_t left = font->ftface->glyph->bitmap_left;
		int32_t top = font->ftface->glyph->bitmap_top;
		uint32_t buffer[width * height];

		for (uint32_t j = 0; j < width * height; ++j)
		{
			uint8_t out = font->ftface->glyph->bitmap.buffer[j];
			buffer[j] = (out == 255 ? out : 0) << 24;
		}

		struct kpBitmap *bitmap = kpCreateBitmap(width, height);
		memcpy(bitmap->px, buffer, sizeof buffer);

		font->glyphs[i].size.x = width;
		font->glyphs[i].size.y = height;
		font->glyphs[i].bearing.x = left;
		font->glyphs[i].bearing.y = top;
		font->glyphs[i].bitmap = bitmap;
		font->glyphs[i].advance = advance;
	}

	return font;
}

void kpDrawFontString(struct kpBitmap *dest, struct kpFont *font, char *msg, int32_t xo, int32_t yo)
{
	int32_t ox = xo;

	for (uint32_t i = 0; i < strlen(msg); ++i)
	{
		struct kpGlyph g = font->glyphs[(uint8_t)(msg[i])];
		struct kpGlyph h = font->glyphs['H'];

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
	for (uint32_t i = 0; i < MAX_CHARS; ++i)
		kpDestroyBitmap(font->glyphs[i].bitmap);

	FT_Done_Face(font->ftface);
	FT_Done_FreeType(font->ftlib);

	FREE(font);
}

/*
 * Probably the most annoying clusterfuck of code in here so far
 */

void kpDrawPanel(struct kpBitmap *dest, struct kpPanel *src)
{
	if (!panelSheet)
		panelSheet = kpLoadBitmap("Project/assets/gui.png");

	int32_t right = (src->x + TILE_SIZE) + src->w * TILE_SIZE,
			bottom = (src->y + TILE_SIZE) + src->h * TILE_SIZE,
			left = src->x, top = src->y;

	kpDrawCroppedBitmap(dest, panelSheet, left, top, TILE_SIZE * 0, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	kpDrawCroppedBitmap(dest, panelSheet, right, top, TILE_SIZE * 1, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);

	for (uint32_t i = 0; i < src->w; ++i)
	{
		int32_t x = src->x + TILE_SIZE + (i * TILE_SIZE);

		kpDrawCroppedBitmap(dest, panelSheet, x, top, TILE_SIZE * 2, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
		kpDrawCroppedBitmap(dest, panelSheet, x, bottom, TILE_SIZE * 2, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);

		for (uint32_t j = 0; j < src->h; ++j)
		{
			int32_t y = src->y + TILE_SIZE + (j * TILE_SIZE);

			kpDrawCroppedBitmap(dest, panelSheet, x, y, TILE_SIZE * 0, TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);
		}
	}

	for (uint32_t j = 0; j < src->h; ++j)
	{
		int32_t y = src->y + TILE_SIZE + (j * TILE_SIZE);

		kpDrawCroppedBitmap(dest, panelSheet, left, y, TILE_SIZE * 3, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
		kpDrawCroppedBitmap(dest, panelSheet, right, y, TILE_SIZE * 2, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	}

	kpDrawCroppedBitmap(dest, panelSheet, left, bottom, TILE_SIZE * 0, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	kpDrawCroppedBitmap(dest, panelSheet, right, bottom, TILE_SIZE * 1, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
}
