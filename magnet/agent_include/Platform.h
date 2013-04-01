#ifndef _Platform_H_
#define _Platform_H_

#if (defined WIN32 || defined _WIN32)
#   undef __WINDOWS__
#   define __WINDOWS__
#   undef __MSDOS__
#   define __MSDOS__
#endif

#if (defined (unix) || defined (__unix__) || defined (_POSIX_SOURCE))
#   if (!defined (__VMS__))
#       undef __UNIX__
#       define __UNIX__
#   endif
#endif

#if (defined (__WINDOWS__))

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#   include <direct.h>
#   include <process.h>
#endif

#if (defined (__UNIX__))
#   include <pthread.h>
#endif

#if (defined (__WINDOWS__))

#elif (defined (__UNIX__))
#endif


#endif
