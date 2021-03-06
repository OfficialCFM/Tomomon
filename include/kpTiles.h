#ifndef _kpTiles_h_
#define _kpTiles_h_

#include "kpCommon.h"
#include "kpBitmap.h"

extern struct kpBitmap *tileSheet;

struct kpTile
{
	uint32_t x, y;
	uint32_t sx, sy;
	bool solid;
};

struct kpMap
{
	struct kpTile *data;
	uint32_t w, h;
};

struct kpMap *kpCreateMap(uint32_t w, uint32_t h);
struct kpMap *kpLoadMap(char *path);
struct kpTile kpGetTile(struct kpMap *map, int32_t x, int32_t y);
void kpDrawMap(struct kpMap *map, struct kpBitmap *dest, struct kpVec2f *cam);
void kpDestroyMap(struct kpMap *map);

#endif /* _kpTiles_h_ */
