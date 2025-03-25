#include "wylrt.h"

#include <stdint.h>
#include <string.h>

wylrterror wyland_make_error(const char *caller, const char *name, const char *what,
                     uint64_t ip, uint64_t *segmbeg, uint64_t *segmend, 
                     uint64_t segmsize, uint64_t thread) {
  wylrterror error;
  error.caller = (char*)caller;
  error.name = (char*)name;
  error.what = (char*)what;
  error.ip = ip;
  error.segmbeg = segmbeg;
  error.segmend = segmend;
  error.segsize = segmsize;
  
  return error;
}

wyland_bool wyland_flags_extract_bool(wyland_flags *flags) {
  if (flags->fmt[flags->pos] == 'b') {
    if (flags->pos + 1 >= flags->len) {
      wylrterror error = wyland_make_error(
        "wyland_flags_extract_bool(wyland_flags *)", 
        "out of range", "reading a boolean, but index is out of range", 
        0, NULL, NULL, 0, 0
      );
      wyland_throw(&error);
    }

    return *(wyland_bool*)flags->objects[flags->pos++];
  }

  wylrterror error = wyland_make_error(
    "wyland_flags_extract_bool(wyland_flags *)", 
    "invalid type convertion", "not a bool (wyland_bool)", 
    0, NULL, NULL, 0, 0
  );

  wyland_throw(&error);

  return wyland_false;
}

wyland_int wyland_flags_extract_int(wyland_flags *flags) {
  if (flags->fmt[flags->pos] == 'i') {
    if (flags->pos + 1 >= flags->len) {
      wylrterror error = wyland_make_error(
        "wyland_flags_extract_int(wyland_flags *)", 
        "out of range", "reading an integer, but index is out of range", 
        0, NULL, NULL, 0, 0
      );
      wyland_throw(&error);
    }

    return *(wyland_int*)flags->objects[flags->pos++];
  }

  wylrterror error = wyland_make_error(
    "wyland_flags_extract_int(wyland_flags *)", 
    "invalid type convertion", "not an int (wyland_int)", 
    0, NULL, NULL, 0, 0
  );

  wyland_throw(&error);

  return -1;
}
