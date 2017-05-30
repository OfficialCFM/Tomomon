#include "kpVector.h"

struct kpVector *kpCreateVector(void)
{
	struct kpVector *vector = ALLOC(struct kpVector);
	vector->capacity = 4;
	vector->total = 0;
	vector->data = ALLOCS(void *, vector->capacity);

	return vector;
}

void kpAddToVector(struct kpVector *vector, void *obj)
{
	if (vector->capacity == vector->total)
	{
		void **data = REALLOC(vector->data, void *, (vector->capacity * 2));

		if (data)
		{
			vector->data = data;
			vector->capacity = vector->capacity * 2;
		}
	}

	vector->data[vector->total++] = obj;
}

void kpSetFromVector(struct kpVector *vector, uint32_t index, void *obj)
{
	if (index >= 0 && index < vector->total)
		vector->data[index] = obj;
}

void *kpGetFromVector(struct kpVector *vector, uint32_t index)
{
	if (index >= 0 && index < vector->total)
		return vector->data[index];

	return NULL;
}

void kpDeleteFromVector(struct kpVector *vector, uint32_t index)
{
	if (index < 0 || index >= vector->total)
		return;

	FREE(vector->data[index]);
	vector->data[index] = NULL;

	for (uint32_t i = index; i < vector->total - 1; ++i)
	{
		vector->data[i] = vector->data[i + 1];
		vector->data[i + 1] = NULL;
	}

	vector->total--;

	if (vector->total > 0 && vector->total == vector->capacity / 4)
	{
		void **data = REALLOC(vector->data, void *, (vector->capacity / 2));

		if (data)
		{
			vector->data = data;
			vector->capacity = vector->capacity / 2;
		}
	}
}

void kpDestroyVector(struct kpVector *vector)
{
	FREE(vector->data);
	FREE(vector);
}
