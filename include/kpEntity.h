#ifndef _kpEntity_h_
#define _kpEntity_h_

#include "kpCommon.h"
#include "kpBitmap.h"
#include "kpError.h"
#include "kpVector.h"
#include "kpTiles.h"

extern struct kpBitmap *entitySheet;
extern struct kpVector *entityList;

enum kpEntityId
{
	KP_ENTITY_PLAYER,
	KP_ENTITY_NPC
};

enum kpEntityDir
{
	KP_UP,
	KP_DOWN,
	KP_LEFT,
	KP_RIGHT
};

struct kpEntity
{
	struct kpMap *map;
	struct kpVec2i pos;
	struct kpVec2i spr;
	struct kpVec2i bnd;
	uint32_t id, dir;
};

void kpAddToEntityList(struct kpVector *list, struct kpEntity *entity);
struct kpEntity *kpFindFromEntityList(struct kpVector *list, enum kpEntityId id);
struct kpEntity *kpGetFromEntityList(struct kpVector *list, uint32_t index);
void kpDeleteFromEntityList(struct kpVector *list, uint32_t index);

struct kpEntity *kpCreateEntity(struct kpMap *map, int32_t x, int32_t y, int32_t sx, int32_t sy, int32_t w, int32_t h, enum kpEntityId id);
void kpDrawEntity(struct kpBitmap *dest, struct kpEntity *src, struct kpVec2f *cam);
void kpMoveEntity(struct kpEntity *entity, int32_t x, int32_t y);
void kpDestroyEntity(struct kpEntity *entity);

#endif /* _kpEntity_h_ */
