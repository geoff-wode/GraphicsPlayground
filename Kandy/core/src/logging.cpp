#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core\logging.h>

namespace
{
  FILE* logFile = NULL;
  Kandy::Core::Logger::Level::Enum currentLevel = Kandy::Core::Logger::Level::All;

  void CloseLogFile()
  {
    fclose(logFile);
  }
}

void Kandy::Core::Logger::Log(Level::Enum level, const char* const fmt, ...)
{
  if (!logFile)
  {
    logFile = fopen("log.txt", "w");
    atexit(CloseLogFile);
    fprintf(logFile, "*\n* Kandy : " __DATE__ " " __TIME__ "\n*\n");
  }

  if (currentLevel & level)
  {
    va_list args;
    va_start(args, fmt);
    vfprintf(logFile, fmt, args);
    va_end(args);
  }
}

void Kandy::Core::Logger::SetLevel(Kandy::Core::Logger::Level::Enum level)
{
  currentLevel = level;
}

Kandy::Core::Logger::Level::Enum Kandy::Core::Logger::GetLevel()
{
  return currentLevel;
}
