#include "kpError.h"

static FILE *logFile = NULL;

/*
 * Turn an error code into something readable
 */

char *kpParseError(enum kpStatus status)
{
	char *msg[] =
	{
		"No error",
		"Invalid memory allocation",
		"Unknown error",

		"Win32 class registration failed",
		"Win32 window creation failed",
		"Win32 device context retrival failed",

		"Invalid video mode width",
		"Invalid video mode height",

		"File not found",

		"Failed to initialize FreeType",
		"Failed to set font dimensions",
		"Failed to load character"
	};

	return msg[status];
}

/*
 * Logger stuff
 */

enum kpStatus kpCreateLogger(void)
{
	char buffer[22], prefix[] = "Log file created ";
	time_t timer;
	int8_t i;

	logFile = fopen("./logfile.txt", "w");

	if (logFile == NULL)
		return KP_FILE_NOT_FOUND;

	time(&timer);
	struct tm tmInfo = *localtime(&timer);
	strftime(buffer, sizeof buffer, "%Y-%m-%d @ %H:%M:%S", &tmInfo);

	fprintf(logFile, "%s%s\n", prefix, buffer);

	for (i = 0; i < strlen(buffer) + strlen(prefix); ++i)
		fprintf(logFile, "=");

	fprintf(logFile, "\n\n");

	return KP_SUCCESS;
}

void kpLogInfo(char *msg, ...)
{
	if (!logFile)
		kpCreateLogger();

	va_list argptr;

	printf("INFO: ");
	fprintf(logFile, "INFO: ");
	va_start(argptr, msg);
	vprintf(msg, argptr);
	vfprintf(logFile, msg, argptr);
	va_end(argptr);
	printf("\n");
	fprintf(logFile, "\n");
	fflush(stdout);
	fflush(logFile);
}

void kpPrintInfo(char *msg, ...)
{
	va_list argptr;

	printf("INFO: ");
	va_start(argptr, msg);
	vprintf(msg, argptr);
	va_end(argptr);
	printf("\n");
	fflush(stdout);
}

void kpLogError(char *msg, ...)
{
	if (!logFile)
		kpCreateLogger();

	va_list argptr;

	printf("ERROR: ");
	fprintf(logFile, "ERROR: ");
	va_start(argptr, msg);
	vprintf(msg, argptr);
	vfprintf(logFile, msg, argptr);
	va_end(argptr);
	printf("\n");
	fprintf(logFile, "\n");
	fflush(stdout);
	fflush(logFile);

#ifdef _WIN32
	char res[256];
	va_start(argptr, msg);
	vsprintf(res, msg, argptr);
	MessageBox(NULL, _T(res), NULL, MB_ICONERROR);
	va_end(argptr);
#endif

	kpQuit(1);
}

void kpPrintError(char *msg, ...)
{
	va_list argptr;

	printf("ERROR: ");
	va_start(argptr, msg);
	vprintf(msg, argptr);
	va_end(argptr);
	printf("\n");
	fflush(stdout);

#ifdef _WIN32
	char res[256];
	va_start(argptr, msg);
	vsprintf(res, msg, argptr);
	MessageBox(NULL, _T(res), NULL, MB_ICONERROR);
	va_end(argptr);
#endif

	kpQuit(1);
}

void kpDestroyLogger(void)
{
	if (!logFile)
		kpCreateLogger();

	fflush(logFile); /* Just in case */
	fclose(logFile);
}

/*
 * Shut down and save log
 */

void kpQuit(int status)
{
	kpDestroyLogger();
	exit(status);
}
