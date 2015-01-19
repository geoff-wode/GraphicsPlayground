#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>
#include <core\logging.h>

using namespace Kandy::Core;

namespace
{
  Logger::Level::Enum currentLevel = Logger::Level::Info;
  FILE* logFile = NULL;

  void CloseFile()
  {
    fclose(logFile);
  }
}

void Kandy::Core::Logger::Log(Level::Enum level, const char* const fmt, ...)
{
  if (!logFile)
  {
    const time_t now = time(NULL);
    const struct tm* timeinfo = localtime(&now);
    char buffer[512];
    strftime(buffer, sizeof(buffer)-1, "%Y-%m-%d:%H:%M:%S", timeinfo);

    logFile = fopen("log.txt", "w");
    fprintf(logFile, "*\n* Kandy : %s\n", buffer);
    atexit(CloseFile);
  }

  if (level <= currentLevel)
  {
    static const char* const infix = "graphicsplayground";
    const char* const fileStart = strstr(fmt, infix);
    const char* const fmtStart = (fileStart != NULL ? fileStart + strlen(infix) : fmt);
    va_list args;
    va_start(args, fmt);
    vfprintf(logFile, fmtStart, args);
    va_end(args);
  }
}

void Logger::SetLevel(Logger::Level::Enum level)
{
  currentLevel = level;
}

Logger::Level::Enum Logger::GetLevel()
{
  return currentLevel;
}
