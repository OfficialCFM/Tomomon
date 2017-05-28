#ifndef _kpError_h_
#define _kpError_h_

#include "kpCommon.h"

enum kpStatus
{
	/* Generic setup codes */
	KP_SUCCESS,
	KP_INVALID_ALLOC,
	KP_UNKNOWN_ERROR,

	/* Framework setup codes */
	KP_WIN32_REGISTRATION_FAILURE,
	KP_WIN32_CREATION_FAILURE,
	KP_WIN32_DC_RETRIEVAL_FAILURE,

	/* Video mode codes */
	KP_VIDEOMODE_INVALID_WIDTH,
	KP_VIDEOMODE_INVALID_HEIGHT,

	/* File loading codes */
	KP_FILE_NOT_FOUND,

	/* Font codes */
	KP_FONT_FREETYPE_FAILURE,
	KP_FONT_DIMENSIONS_FAILURE,
	KP_FONT_FAILED_TO_LOAD_CHAR
};

/* Turn an error code into something readable */
char *kpParseError(enum kpStatus status);

/* Logger stuff */
enum kpStatus kpCreateLogger(void);
void kpLogInfo(char *msg, ...);
void kpPrintInfo(char *msg, ...);
void kpLogError(char *msg, ...);
void kpPrintError(char *msg, ...);
void kpDestroyLogger(void);

/* Shut down and save log */
void kpQuit(int status);

#endif /* _kpError_h_ */
