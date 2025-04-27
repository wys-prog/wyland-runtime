#ifndef __WYLAND_RUNTIME_LIB__
#define __WYLAND_RUNTIME_LIB__

#include <stdint.h>
#include <string.h>

#define WYLAND_NULL 0

typedef struct {
  char     *what;
  char     *name;
  char     *caller;
  char     *exception_type;
  uint64_t  ip;
  uint64_t  thread;
  uint64_t *segmbeg;
  uint64_t *segmend;
  uint64_t  segsize;
} wylrterror;

typedef int8_t      wyland_bool;
typedef int32_t     wyland_int;
typedef int64_t     wyland_long;
typedef uint32_t    wyland_uint;
typedef uint64_t    wyland_ulong;
typedef int8_t      wyland_char;
typedef uint8_t     wyland_uchar;
typedef void*       wyland_ptr;
typedef uint8_t     wyland_byte;

typedef int8_t      wbool;
typedef int32_t     wint;
typedef int64_t     wlong;
typedef uint32_t    wuint;
typedef uint64_t    wulong;
typedef int8_t      wchar;
typedef uint8_t     wuchar;
typedef void*       wptr;
typedef uint8_t     wbyte;

typedef struct {
  uint8_t  (*r8)[16];  
  uint16_t (*r16)[16];  
  uint32_t (*r32)[16];  
  uint64_t (*r64)[32];
} wyland_registers;

typedef struct {
  wyland_uchar     *segstart;
  wyland_uint       seglen; /* Use 32bits since 512MB is under 32 bits's max. */
  wyland_uchar     *keyboardstart;
  wyland_registers *regspointer;
  wyland_ulong      ip;
  wyland_ulong      thread;
  wyland_uchar     *memory_start; /* Use it only with SYSTEM privileges. */
  wyland_bool       is_system;
} arg_t;

#define wyland_func(name, flags) wint name(arg_t *flags)
#define wyland_extern(name, flags) extern wyland_func(name, flags)
#define wyland_extern_cpp(name, flags) extern "C" wyland_func(name, flags)

#define wyland_module_init() wbool Einit()
#define wyland_module_shutdown() void Eshutdown()
#define wyland_module_name() const char *Ename()
#define wyland_module_send(data) void Esend_data(wulong data) 
#define wyland_module_receive() wulong Ereceive_data()

typedef struct {
  void        **objects;
  char        *fmt;
  wyland_uint  len;
  wyland_uint  pos;
} wyland_flags;

typedef wyland_int(*wyland_callable)(wyland_flags*);
typedef void(*wyland_error_block)(const wylrterror*);

#define wyland_true  1
#define wyland_false 0
#define wyland_eof   (-1)
#define weof         wyland_eof

#ifdef __cplusplus
extern "C" {
#endif // C++

void wyland_throw(const wylrterror *error);
wint wyland_try_callable(wyland_callable*, wyland_flags*, wyland_error_block*);

wylrterror wyland_make_error(const char*c, const char*n, const char*w, uint64_t ip, uint64_t*sb, uint64_t*se, uint64_t ss, uint64_t t);

wyland_bool  wyland_flags_extract_bool(wyland_flags*);
wyland_int   wyland_flags_extract_int(wyland_flags*);
wyland_uint  wyland_flags_extract_uint(wyland_flags*);
wyland_long  wyland_flags_extract_long(wyland_flags*);
wyland_ulong wyland_flags_extract_ulong(wyland_flags*);
wyland_char  wyland_flags_extract_char(wyland_flags*);
wyland_uchar wyland_flags_extract_uchar(wyland_flags*);

float        wyland_get_runtime_version(void);

#ifdef __cplusplus
}
#endif // C++
#endif // __WYLAND_RUNTIME_LIB__