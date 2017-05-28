#include "kpGUI.h"

static struct kpBitmap *panelSheet = NULL;

/*
 * Probably the most annoying clusterfuck of code in here so far
 */

void kpDrawPanel(struct kpBitmap *dest, struct kpPanel *src)
{
	uint32_t i, j;

	if (!panelSheet)
		panelSheet = kpLoadBitmap("Project/assets/gui.png");

	kpDrawCroppedBitmap(dest, panelSheet, src->x, src->y, 8 * 0, 8 * 0, 8, 8);
	kpDrawCroppedBitmap(dest, panelSheet, (src->x + 8) + src->w * 8, src->y, 8 * 1, 8 * 0, 8, 8);

	for (i = 0; i < src->w; ++i)
	{
		kpDrawCroppedBitmap(dest, panelSheet, src->x + 8 + (i * 8), src->y, 8 * 2, 8 * 1, 8, 8);
		kpDrawCroppedBitmap(dest, panelSheet, src->x + 8 + (i * 8), (src->y + 8) + src->h * 8, 8 * 2, 8 * 1, 8, 8);

		for (int j = 0; j < src->h; ++j)
			kpDrawCroppedBitmap(dest, panelSheet, src->x + 8 + (i * 8), src->y + 8 + (j * 8), 8 * 0, 8 * 3, 8, 8);
	}

	for (j = 0; j < src->h; ++j)
	{
		kpDrawCroppedBitmap(dest, panelSheet, src->x, src->y + 8 + (j * 8), 8 * 3, 8 * 0, 8, 8);
		kpDrawCroppedBitmap(dest, panelSheet, (src->x + 8) + src->w * 8, src->y + 8 + (j * 8), 8 * 2, 8 * 0, 8, 8);
	}

	kpDrawCroppedBitmap(dest, panelSheet, src->x, (src->y + 8) + src->h * 8, 8 * 0, 8 * 1, 8, 8);
	kpDrawCroppedBitmap(dest, panelSheet, (src->x + 8) + src->w * 8, (src->y + 8) + src->h * 8, 8 * 1, 8 * 1, 8, 8);
}
