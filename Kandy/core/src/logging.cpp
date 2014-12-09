#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core\logging.h>

static FILE* logFile = NULL;

static void CloseLogFile()
{
  fclose(logFile);
}

void Kandy::Core::Logger::Log(const char* const fmt, ...)
{
  if (!logFile)
  {
    logFile = fopen("log.txt", "w");
    atexit(CloseLogFile);
    fprintf(logFile, "*\n* Kandy : " __DATE__ " " __TIME__ "\n*\n");
  }
  va_list args;
  va_start(args, fmt);
  vfprintf(logFile, fmt, args);
  va_end(args);
}
