#include "kpEntity.h"

struct kpBitmap *entitySheet = NULL;
struct kpVector *entityList = NULL;

static bool isTileEqual(struct kpTile *last, struct kpTile *next)
{
	if (last->x == next->x && last->y == next->y &&
		last->sx == next->sx && last->sy == next->sy &&
		last->solid == next->solid)
	{
		return true;
	}

	return false;
}

static bool isSolidTile(struct kpEntity *entity, int32_t xa, int32_t ya, int32_t x, int32_t y)
{
	struct kpTile last = kpGetTile(entity->map, (entity->pos.x + x) / TILE_SIZE, (entity->pos.y + y) / TILE_SIZE);
	struct kpTile next = kpGetTile(entity->map, (entity->pos.x + x + xa) / TILE_SIZE, (entity->pos.y + y + ya) / TILE_SIZE);

	if (!isTileEqual(&last, &next) && next.solid)
		return true;

	return false;
}

static bool isEntityStuck(struct kpEntity *entity, int32_t xa, int32_t ya)
{
	int32_t x0 = 0, x1 = entity->bnd.x - 1,
			y0 = 0, y1 = entity->bnd.y - 1;

	for (uint32_t x = x0; x < x1; ++x)
	{
		if (isSolidTile(entity, xa, ya, x, y0) || isSolidTile(entity, xa, ya, x, y1))
			return true;
	}

	for (uint32_t y = y0; y < y1; ++y)
	{
		if (isSolidTile(entity, xa, ya, x0, y) || isSolidTile(entity, xa, ya, x1, y))
			return true;
	}

	return false;
}

void kpAddToEntityList(struct kpVector *list, struct kpEntity *entity)
{
	kpAddToVector(list, entity);
}

struct kpEntity *kpFindFromEntityList(struct kpVector *list, enum kpEntityId id)
{
	for (uint32_t i = 0; i < list->total; ++i)
	{
		struct kpEntity *entity = kpGetFromEntityList(list, i);

		if (entity->id == id)
			return entity;
	}

	return NULL;
}

struct kpEntity *kpGetFromEntityList(struct kpVector *list, uint32_t id)
{
	return (struct kpEntity *)(list->data[id]);
}

void kpDeleteFromEntityList(struct kpVector *list, uint32_t index)
{
	kpDeleteFromVector(list, index);
}

struct kpEntity *kpCreateEntity(struct kpMap *map, int32_t x, int32_t y, int32_t sx, int32_t sy, int32_t w, int32_t h, enum kpEntityId id)
{
	struct kpEntity *entity = ALLOC(struct kpEntity);
	entity->map = map;
	entity->pos.x = x * TILE_SIZE;
	entity->pos.y = y * TILE_SIZE;
	entity->spr.x = sx; entity->spr.y = sy;
	entity->bnd.x = w; entity->bnd.y = h;
	entity->id = id;
	entity->dir = KP_DOWN;

	return entity;
}

void kpDrawEntity(struct kpBitmap *dest, struct kpEntity *src, struct kpVec2f *cam)
{
	if (!entitySheet)
		entitySheet = kpLoadBitmap("Project/assets/ent.png");

	if (cam)
		kpDrawCroppedBitmap(dest, entitySheet, src->pos.x - cam->x, src->pos.y - cam->y, src->spr.x * TILE_SIZE, src->spr.y * TILE_SIZE, src->bnd.x, src->bnd.y);
	else
		kpDrawCroppedBitmap(dest, entitySheet, src->pos.x, src->pos.y, src->spr.x * TILE_SIZE, src->spr.y * TILE_SIZE, src->bnd.x, src->bnd.y);
}

void kpMoveEntity(struct kpEntity *entity, int32_t x, int32_t y)
{
	/* TODO: Real Pokemon-styled movement */

	if (x != 0 && y != 0)
	{
		kpMoveEntity(entity, x, 0);
		kpMoveEntity(entity, 0, y);

		return;
	}

	if (!isEntityStuck(entity, x, y))
	{
		if (y < 0) entity->dir = KP_UP;
		if (y > 0) entity->dir = KP_DOWN;
		if (x < 0) entity->dir = KP_LEFT;
		if (x > 0) entity->dir = KP_RIGHT;

		entity->pos.x += x;
		entity->pos.y += y;
	}
}

void kpDestroyEntity(struct kpEntity *entity)
{
	FREE(entity);
}
