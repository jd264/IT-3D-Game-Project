#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

FILE * __logFile = NULL;

void closeLogger()
{
	if(__logFile !=NULL)
	{
		fclose(__logFile);
		__logFile = NULL;
	}
}

void loggerInit(const char *logPath)
{
	if(logPath == NULL)
	{
		__logFile = fopen("output.og", "a");
	}
	else
	{
		__logFile = fopen(logPath, "a");
	}
	atexit(closeLogger);
}

void _slog(char *f, int l, char *msg,...)
{
	va_list ap;
	/*echo*/
	va_start(ap,msg);
    fprintf(stdout,"%s:%i: ",f,l);
    vfprintf(stdout,msg,ap);
    fprintf(stdout,"\n");
    va_end(ap);
    if (__logFile != NULL)
    {
        va_start(ap,msg);
        fprintf(__logFile,"%s:%i: ",f,l);
        vfprintf(__logFile,msg,ap);
        fprintf(__logFile,"\n");
        va_end(ap);
    }
}

/*eol@eof*/