#include "kpTiles.h"

#define TILE_SIZE 8

static struct kpBitmap *tileSheet = NULL;

struct kpMap *kpCreateMap(uint32_t w, uint32_t h)
{
	uint32_t i, j;

	struct kpMap *map = ALLOC(struct kpMap);
	map->data = ALLOCS(struct kpTile, w * h);
	map->w = w; map->h = h;

	for (i = 0; i < w; ++i)
	{
		for (j = 0; j < h; ++j)
		{
			map->data[i + j * w].x = i;
			map->data[i + j * w].y = j;

			if (i == j)
				map->data[i + j * w].sx = 1;
		}
	}

	return map;
}

struct kpMap *kpLoadMap(char *path)
{
	/* Implement this later */
	return NULL;
}

void kpDrawMap(struct kpMap *map, struct kpBitmap *dest, struct kpVec2f *cam)
{
	uint32_t i, j;

	if (!tileSheet)
		tileSheet = kpLoadBitmap("Project/assets/map.png");

	for (i = 0; i < map->w; ++i)
	{
		for (j = 0; j < map->h; ++j)
		{
			struct kpTile tile = map->data[i + j * map->w];
			kpDrawCroppedBitmap(dest, tileSheet, (tile.x * TILE_SIZE) - cam->x, (tile.y * TILE_SIZE) - cam->y, tile.sx * TILE_SIZE, tile.sy * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
	}
}

void kpDestroyMap(struct kpMap *map)
{
	FREE(map->data);
	FREE(map);
}
