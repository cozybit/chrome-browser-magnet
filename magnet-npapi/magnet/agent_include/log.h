#ifndef __LOG_H__
#define __LOG_H__

#ifdef __ANDROID__
#   include <android/log.h>
#   define LOG_ALWAYS(level, application, fmt, ...) __android_log_print(level, application, fmt, ##__VA_ARGS__)
#else
#   define LOG_ALWAYS(level, application, fmt, ...) fprintf (stderr, "[libmagnet] "fmt"\n", ##__VA_ARGS__)
#endif

#ifndef DEBUG_OUTPUT
#    define MAGNET_LOG_FATAL   0
#    define MAGNET_LOG_ERROR   0
#    define MAGNET_LOG_VERBOSE 0
#    define MAGNET_LOG_DEBUG 0

#    define LOG_PRINT(level, application, fmt, ...)
#    define LOG_PRINT_DEBUG(level, application, fmt, ...)
#    define LOG_WRITE(level, application, message)
#else

#    ifdef __ANDROID__
#        define MAGNET_LOG_FATAL   ANDROID_LOG_FATAL
#        define MAGNET_LOG_ERROR   ANDROID_LOG_ERROR
#        define MAGNET_LOG_VERBOSE ANDROID_LOG_VERBOSE
#        define MAGNET_LOG_DEBUG ANDROID_LOG_DEBUG

#        define LOG_PRINT(level, application, fmt, ...) __android_log_print(level, application, fmt, ##__VA_ARGS__)
#        define LOG_PRINT_DEBUG(level, application, fmt, ...) __android_log_print(level, application, fmt, ##__VA_ARGS__)
#        define LOG_WRITE(level, application, message) __android_log_write(level, application, message)
#    else
#        define MAGNET_LOG_FATAL   0
#        define MAGNET_LOG_ERROR   0
#        define MAGNET_LOG_VERBOSE 0
#        define MAGNET_LOG_DEBUG 0

#        define LOG_PRINT(level, application, fmt, ...) fprintf (stderr, "[libmagnet] "fmt"\n", ##__VA_ARGS__)
#        define LOG_PRINT_DEBUG(level, application, fmt, ...) fprintf (stderr, "[libmagnet] "fmt"\n", ##__VA_ARGS__)
#        define LOG_WRITE(level, application, message) fprintf (stderr, "[libmagnet] "message"\n")
#    endif
#endif


#endif
