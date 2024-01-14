/**
 * @file _fn-io.c
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "fn-io.h"

void WSP_DebugPrintf(const char *fmt, ...)
{
#ifdef _DEBUG
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
#endif
}
void WSP_Error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fflush(stdout);
    fflush(stderr);
}
void WSP_Warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fflush(stdout);
    fflush(stderr);
}

void WSP_Printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    fflush(stdout);
}

bool WSP_Exists(const char *path)
{
    return WSP_ExistsFile(path) || WSP_ExistsDirectory(path);
}

bool WSP_ExistsFile(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL) { return false; }
    fclose(fp);
    return true;
}

bool WSP_ExistsDirectory(const char *path)
{
    struct stat info;
    if (stat(path, &info) != 0)
    {
        return false;
    }
    else if (info.st_mode & S_IFDIR)
    {
        return true;
    }
    else
    {
        return false;
    }
}
