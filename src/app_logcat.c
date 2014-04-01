#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
extern "C"{
#endif

#define NC_LOG_TAG "bttest"

#define MAX_LOG_SIZE (4098)

void bt_logcat(const char* tag, char* fmt, ...)
{
    va_list argp;
    char tmpLog[MAX_LOG_SIZE];

    memset(tmpLog, 0, sizeof(tmpLog));
    va_start( argp, fmt );
    vsnprintf(tmpLog, MAX_LOG_SIZE, fmt, argp);
    va_end( argp );

    printf("[%s]: %s\n", tag, tmpLog);
    fflush(stdout);
}

#ifdef __cplusplus
}
#endif
