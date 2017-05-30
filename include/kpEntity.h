#ifndef _kpEntity_h_
#define _kpEntity_h_

#include "kpCommon.h"
#include "kpBitmap.h"
#include "kpError.h"
#include "kpVector.h"

extern struct kpBitmap *entitySheet;
extern struct kpVector *entityList;

enum kpEntityId
{
	KP_ENTITY_PLAYER,
	KP_ENTITY_NPC
};

struct kpEntity
{
	enum kpEntityId id;
	struct kpVec2i pos;
	struct kpVec2i spr;
	struct kpVec2i bnd;
};

void kpAddToEntityList(struct kpVector *list, struct kpEntity *entity);
struct kpEntity *kpFindFromEntityList(struct kpVector *list, enum kpEntityId id);
struct kpEntity *kpGetFromEntityList(struct kpVector *list, uint32_t index);
void kpDeleteFromEntityList(struct kpVector *list, uint32_t index);

struct kpEntity *kpCreateEntity(int32_t x, int32_t y, int32_t sx, int32_t sy, int32_t w, int32_t h, enum kpEntityId id);
void kpDrawEntity(struct kpBitmap *dest, struct kpEntity *src, struct kpVec2f *cam);
void kpDestroyEntity(struct kpEntity *entity);

#endif /* _kpEntity_h_ */
