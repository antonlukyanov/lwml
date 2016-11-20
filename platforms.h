// Определения макросов, специфичных для компиляторов и платформ.
// Anton Lukyanov, 2016.

#ifndef _PLATFORMS_
#define _PLATFORMS_

#define OS_WIN   0
#define OS_OSX   0
#define OS_LINUX 0

#if defined(_WIN32) || defined(__MINGW32__ ) || defined(__MINGW64__)
  #undef  OS_WIN
  #define OS_WIN 1
#elif defined(__APPLE__)
  #undef  OS_OSX
  #define OS_OSX 1
#elif defined(__linux__)
  #undef  OS_LINUX
  #define OS_LINUX 1
#endif

#if OS_WIN
  #ifdef __GNUC__
    #define LWML_EXPORT                __attribute__((dllexport))
    #define LWML_IMPORT                __attribute__((dllimport))
    #define UNUSED                     __attribute__((unused))
    #define PACKED                     __attribute__((packed))
    #define PRINTF( fmt_idx, arg_idx ) __attribute__((format(printf, fmt_idx, arg_idx)));
    #define DEPRECATED                 __attribute__((deprecated))
  #else
    #define LWML_EXPORT                __declspec(dllexport)
    #define LWML_IMPORT                __declspec(dllimport)
    #define UNUSED
    #define PACKED
    #define PRINTF( fmt_idx, arg_idx )
    #define DEPRECATED                 __declspec(deprecated)
  #endif
#else
  #define LWML_EXPORT                  __attribute__((visibility("default")))
  #define LWML_IMPORT                  __attribute__((visibility("hidden")))
  #define UNUSED                       __attribute__((unused))
  #define PACKED                       __attribute__((packed))
  #define PRINTF( fmt_idx, arg_idx )   __attribute__((format(printf, fmt_idx, arg_idx)));
  #define DEPRECATED                   __attribute__((deprecated))
#endif

#ifdef LWML_DYNAMIC
  #define LWML_API LWML_EXPORT
#else
  #define LWML_API LWML_IMPORT
#endif

#endif // _PLATFORMS_
