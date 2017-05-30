#include "kpEngine.h"

/*
 * Yes, this file is a mess.
 * It's all for testing.
 *
 * TODO: Move the player entity outside of the list,
 *       that way you have more control over how it's used.
 *
 */

static int ticks = 0;
static struct kpBitmap *test = NULL;
static struct kpFont *fnt = NULL;
static struct kpMap *map = NULL;
static struct kpVec2f cam = { 0.0f, 0.0f };
static struct kpVector *list;

struct kpEngine *kpCreateEngine(enum kpStatus *status)
{
	struct kpEngine *engine = ALLOC(struct kpEngine);
	engine->bmp = kpCreateBitmap(BMP_W, BMP_H);
	engine->running = true;

	if (!engine || !engine->bmp)
		*status = KP_INVALID_ALLOC;
	else
		*status = KP_SUCCESS;

	test = kpLoadBitmap("Project/assets/map.png");
	fnt = kpLoadFont("Project/assets/font.ttf", 8);
	map = kpCreateMap(15, 15);

	list = kpCreateVector();
	kpAddToEntityList(list, kpCreateEntity(2, 2, 0, 0, TILE_SIZE * 2, TILE_SIZE * 2, KP_ENTITY_PLAYER));

	return engine;
}

void kpEngineTick(struct kpEngine *engine)
{
	ticks++;

	struct kpEntity *player = kpFindFromEntityList(list, KP_ENTITY_PLAYER);

	if (player) /* TODO: Actual Pokemon-styled movement */
	{
		if (engine->keys[KP_KEY_W])	player->pos.y--;
		if (engine->keys[KP_KEY_S])	player->pos.y++;
		if (engine->keys[KP_KEY_A]) player->pos.x--;
		if (engine->keys[KP_KEY_D])	player->pos.x++;
	}

	if (engine->keys[KP_KEY_R])
	{
		kpAddToVector(list, kpCreateEntity((rand() % 14), (rand() % 14), 0, 0, 16, 16, KP_ENTITY_NPC));
		engine->keys[KP_KEY_R] = false;
	}

	if (engine->keys[KP_KEY_F] && list->total > 1)
	{
		kpDeleteFromEntityList(list, list->total - 1);
		engine->keys[KP_KEY_F] = false;
	}
}

void kpEngineRender(struct kpEngine *engine)
{
	struct kpEntity *player = kpFindFromEntityList(list, KP_ENTITY_PLAYER);
	cam.x = player->pos.x - ((BMP_W - player->bnd.x) / 2);
	cam.y = player->pos.y - ((BMP_H - player->bnd.y) / 2) + (((4 * 8)) / 2);

	struct kpPanel panel = { 0, BMP_H - (6 * 8), 18, 4 };

	kpClearBitmap(engine->bmp, 0);
	kpDrawMap(map, engine->bmp, &cam);

	for (uint32_t i = 0; i < list->total; ++i)
	{
		struct kpEntity *e = list->data[i];
		kpDrawEntity(engine->bmp, e, &cam);
	}

	kpDrawPanel(engine->bmp, &panel);
	kpDrawFontString(engine->bmp, fnt, "Welcome to the\nworld of TOMOMON!", 9, 17 + BMP_H - 6 * 8);
}

void kpDestroyEngine(struct kpEngine *engine)
{
	/* Game resources */
	kpDestroyMap(map);
	kpDestroyBitmap(test);
	kpDestroyFont(fnt);
	kpDestroyVector(list);

	/* Engine resources */
	kpDestroyBitmap(engine->bmp);
	FREE(engine);
}
