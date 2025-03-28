#ifndef __WYLAND_RUNTIME_LIB__
#define __WYLAND_RUNTIME_LIB__

#include <stdint.h>
#include <string.h>

typedef struct {
  char    *what;
  char    *name;
  char    *caller;
  char    *exception_type;
  uint64_t ip;
  uint64_t thread;
  uint64_t *segmbeg;
  uint64_t *segmend;
  uint64_t  segsize;
} wylrterror;

typedef int8_t      wyland_bool;
typedef int32_t     wyland_int;
typedef int64_t     wyland_long;
typedef uint32_t    wyland_uint;
typedef uint64_t    wyland_ulong;

typedef struct {
  void        **objects;
  char        *fmt;
  wyland_uint  len;
  wyland_uint  pos;
} wyland_flags;

typedef wyland_int(*wyland_callable)(wyland_flags*);

static const wyland_bool wyland_true = 1;
static const wyland_bool wyland_false = 0;

#ifdef __cplusplus
extern "C" {
#endif // C++

void wyland_throw(const wylrterror *error);

wylrterror wyland_make_error(const char*, const char*, const char*, uint64_t, uint64_t*, uint64_t*, uint64_t, uint64_t);

wyland_bool  wyland_flags_extract_bool(wyland_flags*);
wyland_int   wyland_flags_extract_int(wyland_flags*);
wyland_uint  wyland_flags_extract_uint(wyland_flags*);
wyland_long  wyland_flags_extract_long(wyland_flags*);
wyland_ulong wyland_flags_extract_ulong(wyland_flags*);

  
#ifdef __cplusplus
}
#endif // C++
#endif // __WYLAND_RUNTIME_LIB__