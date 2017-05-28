#ifndef _kpBitmap_h_
#define _kpBitmap_h_

#include "kpCommon.h"
#include "kpError.h"

struct kpBitmap
{
	int32_t w, h;
	uint32_t *px;
};

uint32_t kpBlendPix(uint32_t s0, uint32_t s1);

struct kpBitmap *kpCreateBitmap(int32_t w, int32_t h);
struct kpBitmap *kpLoadBitmap(char *path);
void kpDestroyBitmap(struct kpBitmap *bitmap);

void kpDrawBitmap(struct kpBitmap *dest, struct kpBitmap *src, int32_t xo, int32_t yo);
void kpDrawCroppedBitmap(struct kpBitmap *dest, struct kpBitmap *src, int32_t xo, int32_t yo, int32_t xc, int32_t yc, int32_t w, int32_t h);
void kpClearBitmap(struct kpBitmap *bitmap, uint32_t color);

#endif /* _kpBitmap_h_ */
