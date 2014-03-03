/*
 * SidDebugging.hpp
 *
 *  Created on: Dec 21, 2009
 *      Author: lauri
 */

#ifndef SIDDEBUGGING_HPP_
#define SIDDEBUGGING_HPP_

#include <stdio.h>
#include <string.h>
#include <time.h>
#ifndef _WIN32
#include <sys/time.h>
#endif

#ifdef _WIN32
#define __func__ __FUNCTION__
#endif

/*
 * Don't include this in header files. Only in cpp files
 *
 * input defines:
 * SID_DBGPRINTF - printf like logging function. Has to be defined in SidPlatform.hpp
 * SID_ENABLE_DEBUG - enable / disable all debugging
 * SID_LOG_MASK - log mask for debug macros
 * SID_LOG_PREFIX - log prefix for debug strings
 * SID_DBG_TIMESTAMPS - whether or not to include timestamps
 *
 * defines macros:
 * SID_FATAL - fatal error, also exits the program
 * SID_ERROR, SID_WARN, SID_INFO, SID_DBG, SID_VDBG - debug printf-s, activated according to log mask
 * SID_FUNCLOG - scoped function logging macro
 * */

#ifdef SID_SKYPEKIT_SIDE

#undef SID_DBGPRINTF
#include "Debugging/Logging.hpp"
#define SID_DBGPRINTF(format, ...) { REALTIME_LOG("SID" format, ##__VA_ARGS__); printf(format, ##__VA_ARGS__); }

#else

#ifdef ANDROID
#undef SID_DBGPRINTF
#include <android/log.h>
#define SID_DBGPRINTF(format, ...) __android_log_print(ANDROID_LOG_VERBOSE, "SID", format, ##__VA_ARGS__)
#endif

#endif

void _sid_dbgprintf(const char* format, ...);

#ifndef SID_DBGPRINTF
#ifdef _WIN32
#define SID_DBGPRINTF _sid_dbgprintf   
#else 
#define SID_DBGPRINTF(format, ...) printf(format, ##__VA_ARGS__)
#endif
#endif

#ifndef SID_ENABLE_DEBUG
#define SID_ENABLE_DEBUG 1
#endif

#ifndef SID_LOG_MASK
#define SID_LOG_MASK 0x7
#endif

#ifndef SID_LOG_PREFIX
#define SID_LOG_PREFIX ""
#endif

#ifndef SID_DBG_TIMESTAMPS
#define SID_DBG_TIMESTAMPS 0
#endif

#ifndef _WIN32
#if SID_ENABLE_DEBUG == 1

#define _SID_DBG(use_timestamps, fmt, ...) \
	do { \
		if (use_timestamps) { \
			time_t tim=time(NULL); \
			struct tm *t=localtime(&tim); \
			/* timespec ts; clock_gettime(CLOCK_REALTIME, &ts); SID_DBGPRINTF("%02d:%02d:%02d %ld " fmt "\n", t->tm_hour, t->tm_min, t->tm_sec, ts.tv_nsec, ##args); */ \
			SID_DBGPRINTF("%02d:%02d:%02d " fmt "\n", t->tm_hour, t->tm_min, t->tm_sec, ##__VA_ARGS__); \
		} else {\
			SID_DBGPRINTF(fmt "\n", ##__VA_ARGS__); \
		} \
	} while (0)

/// Function logger, used through FUNCLOG macro
struct SidFuncLog
{
	inline SidFuncLog (const char *func_, const char *prefix_, unsigned int useTimestamps_, unsigned int logMask_, unsigned int logLevel_) :
		func(func_),
		prefix(prefix_),
		logMask(logMask_),
		logLevel(logLevel_),
		useTimestamps(useTimestamps_)
	{
		if (logMask & logLevel)
			_SID_DBG(useTimestamps, "%s%s", prefix, func);
	}
	inline ~SidFuncLog() {
		if (logMask & logLevel)
			_SID_DBG(useTimestamps, "%s%s --end", prefix, func);
	}
	private:
		const char *func, *prefix;
		unsigned int logMask, logLevel, useTimestamps;
};


#define _SID_LOG_LONG(use_timestamps, level, file, line, func, fmt, ...) \
	if (SID_LOG_MASK & level) { \
		const char * _file, * tmp = strrchr(file, '/'); \
		if (tmp == NULL) _file=file; else _file=tmp + 1; \
		_SID_DBG(use_timestamps, SID_LOG_PREFIX "%s:%d(%s): " fmt, _file, line, func, ##__VA_ARGS__); \
	}

#define _SID_LOG(use_timestamps, level, fmt, ...) \
	if (SID_LOG_MASK & level) { \
		_SID_DBG(use_timestamps, SID_LOG_PREFIX fmt, ##__VA_ARGS__); \
	}

#define _SID_FUNCLOG(prefix, level) SidFuncLog __l(__func__, prefix, SID_DBG_TIMESTAMPS, SID_LOG_MASK, level)
#else
#define _SID_LOG_LONG(args, ...)
#define _SID_LOG(args, ...)
#define _SID_FUNCLOG(prefix, level)
#endif

#define SID_FATAL(fmt, ...) do { _SID_LOG_LONG(SID_DBG_TIMESTAMPS, 0x1, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); assert(0); abort();} while (0)
#define SID_ERROR(fmt, ...) _SID_LOG_LONG(SID_DBG_TIMESTAMPS, 0x1, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define SID_WARN(fmt, ...) _SID_LOG(SID_DBG_TIMESTAMPS, 0x2, fmt, ##__VA_ARGS__)
#define SID_INFO(fmt, ...) _SID_LOG(SID_DBG_TIMESTAMPS, 0x4, fmt, ##__VA_ARGS__)
#define SID_DBG(fmt, ...) _SID_LOG(SID_DBG_TIMESTAMPS, 0x8, fmt, ##__VA_ARGS__)
#define SID_VDBG(fmt, ...) _SID_LOG(SID_DBG_TIMESTAMPS, 0x10, fmt, ##__VA_ARGS__)

#define SID_FUNCLOG() _SID_FUNCLOG("FUNCLOG:", 0x8)
#endif //_WIN32

#endif /* SIDDEBUGGING_HPP_ */
