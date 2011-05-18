// Определения макросов, специфичных для компиляторов
// lwml, (c) ltwood

#ifndef _PLATFORMS_
#define _PLATFORMS_

#ifdef __GNUC__

  #define LWML_EXPORT extern "C"     __attribute__((dllexport))
  #define LWML_IMPORT extern "C"     __attribute__((dllimport))

  #define UNUSED                     __attribute__((unused))
  #define PACKED                     __attribute__((packed))
  #define PRINTF( fmt_idx, arg_idx ) __attribute__ ((format(printf, fmt_idx, arg_idx)));
  #define DEPRECATED                 __attribute__((deprecated))

#else

  #define LWML_EXPORT extern "C"     __declspec(dllexport)
  #define LWML_IMPORT extern "C"     __declspec(dllimport)

  #define UNUSED 
  #define PACKED
  #define PRINTF( fmt_idx, arg_idx )
  #define DEPRECATED                 __declspec(deprecated)

#endif

#endif // _PLATFORMS_
