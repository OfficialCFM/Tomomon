#ifndef _kpGUI_h_
#define _kpGUI_h_

#include "kpCommon.h"
#include "kpBitmap.h"

struct kpBitmap;

struct kpPanel
{
	int32_t x, y;
	uint32_t w, h;
};

void kpDrawPanel(struct kpBitmap *dest, struct kpPanel *src);

#endif /* _kpGUI_h_ */
