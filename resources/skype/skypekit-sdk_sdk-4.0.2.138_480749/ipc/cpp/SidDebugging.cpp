#include "SidDebugging.hpp"
#include <stdarg.h>

void _sid_dbgprintf(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
}

