#ifndef _kpEngine_h_
#define _kpEngine_h_

#include "kpCommon.h"
#include "kpError.h"
#include "kpBitmap.h"
#include "kpInput.h"
#include "kpTiles.h"
#include "kpGui.h"

struct kpEngine
{
	struct kpBitmap *bmp;
	bool keys[256];
	bool running;
};

struct kpEngine *kpCreateEngine(enum kpStatus *status);
void kpEngineTick(struct kpEngine *engine);
void kpEngineRender(struct kpEngine *engine);
void kpDestroyEngine(struct kpEngine *engine);

#endif /* _kpEngine_h_ */