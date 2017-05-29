#ifndef _kpEngine_h_
#define _kpEngine_h_

#include "kpCommon.h"
#include "kpBitmap.h"
#include "kpError.h"
#include "kpInput.h"
#include "kpInterface.h"
#include "kpTiles.h"

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
