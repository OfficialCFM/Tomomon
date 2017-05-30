#include "kpEntity.h"

struct kpBitmap *entitySheet = NULL;
struct kpVector *entityList = NULL;

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

struct kpEntity *kpCreateEntity(int32_t x, int32_t y, int32_t sx, int32_t sy, int32_t w, int32_t h, enum kpEntityId id)
{
	struct kpEntity *entity = ALLOC(struct kpEntity);
	entity->pos.x = x * TILE_SIZE;
	entity->pos.y = y * TILE_SIZE;
	entity->spr.x = sx; entity->spr.y = sy;
	entity->bnd.x = w; entity->bnd.y = h;
	entity->id = id;

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

void kpDestroyEntity(struct kpEntity *entity)
{
	FREE(entity);
}
