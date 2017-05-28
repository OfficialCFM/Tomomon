#include "kpEngine.h"

static int ticks = 0;
static struct kpBitmap *test = NULL;
static struct kpFont *fnt = NULL;
static struct kpMap *map = NULL;
static struct kpVec2f cam = { 0.0f, 0.0f };

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
	map = kpCreateMap(20, 16);

	return engine;
}

void kpEngineTick(struct kpEngine *engine)
{
	ticks++;

	if (engine->keys[KP_KEY_W])
		cam.y -= 1;
	if (engine->keys[KP_KEY_S])
		cam.y += 1;
	if (engine->keys[KP_KEY_A])
		cam.x -= 1;
	if (engine->keys[KP_KEY_D])
		cam.x += 1;
}

void kpEngineRender(struct kpEngine *engine)
{
	kpClearBitmap(engine->bmp, 0x000000);

	struct kpPanel panel = { 0, BMP_H - (6 * 8), 18, 4 };
	kpDrawMap(map, engine->bmp, &cam);
	kpDrawPanel(engine->bmp, &panel);
	kpDrawFontString(engine->bmp, fnt, "Welcome to the\nworld of TOMOMON!", 9, 17 + BMP_H - 6 * 8);
}

void kpDestroyEngine(struct kpEngine *engine)
{
	/* Game resources */
	kpDestroyMap(map);
	kpDestroyBitmap(test);
	kpDestroyFont(fnt);

	/* Engine resources */
	kpDestroyBitmap(engine->bmp);
	FREE(engine);
}
