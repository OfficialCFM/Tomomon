#ifndef _kpCommon_h_
#define _kpCommon_h_

/* C standard library */
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* C system library */
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

/* Win32 library */
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <tchar.h>
#endif

/* Default window dimensions */
#define WIN_W 640
#define WIN_H 576
#define BMP_W 160 /* WIN_W / 4 */
#define BMP_H 144 /* WIN_H / 4 */

/* To assist in memory allocation */
#define ALLOC(type) calloc(1, sizeof(type))
#define ALLOCS(type, size) calloc(size, sizeof(type))
#define FREE(ptr) free(ptr); ptr = NULL

/* Widely used structs */
struct kpVec2i
{
	int x, y;
};

struct kpVec2f
{
	float x, y;
};

#endif /* _kpCommon_h_ */
