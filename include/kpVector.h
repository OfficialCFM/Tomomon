#ifndef _kpVector_h_
#define _kpVector_h_

#include "kpCommon.h"

/*
 * Essentially just a stripped down std::vector
 */
struct kpVector
{
	void **data;
	uint32_t capacity;
	uint32_t total;
};

struct kpVector *kpCreateVector(void);
void kpAddToVector(struct kpVector *vector, void *obj);
void kpSetFromVector(struct kpVector *vector, uint32_t index, void *obj);
void *kpGetFromVector(struct kpVector *vector, uint32_t index);
void kpDeleteFromVector(struct kpVector *vector, uint32_t index);
void kpDestroyVector(struct kpVector *vector);

#endif /* _kpVector_h_ */
