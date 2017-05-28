#include "kpBitmap.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

uint32_t kpBlendPix(uint32_t s0, uint32_t s1)
{
	int32_t masks[4];
	masks[0] = 0xFF000000;
	masks[1] = 0x00FF00FF;
	masks[2] = 0xFF00FF00;
	masks[3] = 0x0000FF00;

	uint32_t a = (s1 & masks[0]) >> 24;
	uint32_t rb = ((0xFF - a) * (s0 & masks[1]) + (a * (s1 & masks[1]))) >> 8;
	uint32_t ag = ((0xFF - a) * (s0 & masks[2] >> 8)) + (a * (1 << 24 | (s1 & masks[3]) >> 8));

	return (rb & masks[1]) | (ag & masks[2]);
}

struct kpBitmap *kpCreateBitmap(int32_t w, int32_t h)
{
	struct kpBitmap *bitmap = ALLOC(struct kpBitmap);
	bitmap->w = w; bitmap->h = h;
	bitmap->px = ALLOCS(int32_t, w * h);

	return bitmap;
}

struct kpBitmap *kpLoadBitmap(char *path)
{
	struct kpBitmap *bitmap = ALLOC(struct kpBitmap);
	uint8_t *px = stbi_load(path, &bitmap->w, &bitmap->h, NULL, 4);
	uint32_t i, j, k;

	if (!px)
		kpLogError("%s: \"%s\"", kpParseError(KP_FILE_NOT_FOUND), path);

	bitmap->px = ALLOCS(uint32_t, bitmap->w * bitmap->h);

	for (i = 0; i < bitmap->w; ++i)
	{
		for (j = 0; j < bitmap->h; ++j)
		{
			/* Extract each RGB value from STB's output */
			uint8_t z[4];
			for (k = 0; k < 4; ++k)
				z[k] = px[(i + j * bitmap->w) * 4 + k];

			/* Convert it to a single integer */
			uint32_t res = z[0] << 16 | z[1] << 8 | z[2] | z[3] << 24;

			/* Send it to the bitmap */
			bitmap->px[i + j * bitmap->w] = res;
		}
	}

	stbi_image_free(px);

	return bitmap;
}

void kpDestroyBitmap(struct kpBitmap *bitmap)
{
	FREE(bitmap->px);
	FREE(bitmap);
}

void kpDrawBitmap(struct kpBitmap *dest, struct kpBitmap *src, int32_t xo, int32_t yo)
{
	for (uint32_t x = 0; x < src->w; ++x)
	{
		int32_t xp = x + xo;

		if (xp < 0 || xp >= dest->w)
			continue;

		for (uint32_t y = 0; y < src->h; ++y)
		{
			int32_t yp = y + yo;

			if (yp < 0 || yp >= dest->h)
				continue;

			uint32_t res = src->px[x + y * src->w];
			dest->px[xp + yp * dest->w] = kpBlendPix(dest->px[xp + yp * dest->w], res);
		}
	}
}

void kpDrawCroppedBitmap(struct kpBitmap *dest, struct kpBitmap *src, int32_t xo, int32_t yo, int32_t xc, int32_t yc, int32_t w, int32_t h)
{
	for (uint32_t x = 0; x < w; ++x)
	{
		int32_t xp = x + xo;

		if (xp < 0 || xp >= dest->w)
			continue;

		for (uint32_t y = 0; y < h; ++y)
		{
			int32_t yp = y + yo;

			if (yp < 0 || yp >= dest->h)
				continue;

			uint32_t res = src->px[(x + xc) + (y + yc) * src->w];
			dest->px[xp + yp * dest->w] = kpBlendPix(dest->px[xp + yp * dest->w], res);
		}
	}
}

void kpClearBitmap(struct kpBitmap *bitmap, uint32_t color)
{
	for (uint32_t i = 0; i < bitmap->w * bitmap->h; ++i)
		bitmap->px[i] = color;
}
