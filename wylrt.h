#ifndef __WYLAND_RUNTIME_LIB__
#define __WYLAND_RUNTIME_LIB__

#include <stdint.h>

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

static const wyland_bool wyland_true = 1;
static const wyland_bool wyland_flase = 0;

wyland_bool wyland_runtime_init() {

  return 1;
}

#endif // __WYLAND_RUNTIME_LIB__