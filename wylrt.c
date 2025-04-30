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
  error.thread = thread; 
  
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

wyland_uint wyland_flags_extract_uint(wyland_flags *flags) {
  if (flags->fmt[flags->pos] == 'u') {
    if (flags->pos + 1 >= flags->len) {
      wylrterror error = wyland_make_error(
        "wyland_flags_extract_uint(wyland_flags *)", 
        "out of range", "reading an unsigned integer, but index is out of range", 
        0, NULL, NULL, 0, 0
      );
      wyland_throw(&error);
    }

    return *(wyland_uint*)flags->objects[flags->pos++];
  }

  wylrterror error = wyland_make_error(
    "wyland_flags_extract_uint(wyland_flags *)", 
    "invalid type convertion", "not an uint (wyland_uint)", 
    0, NULL, NULL, 0, 0
  );

  wyland_throw(&error);

  return -1;
}

wyland_long wyland_flags_extract_long(wyland_flags *flags) {
  if (flags->fmt[flags->pos] == 'l') {
    if (flags->pos + 1 >= flags->len) {
      wylrterror error = wyland_make_error(
        "wyland_flags_extract_long(wyland_flags *)", 
        "out of range", "reading a long, but index is out of range", 
        0, NULL, NULL, 0, 0
      );
      wyland_throw(&error);
    }

    return *(wyland_long*)flags->objects[flags->pos++];
  }

  wylrterror error = wyland_make_error(
    "wyland_flags_extract_long(wyland_flags *)", 
    "invalid type convertion", "not a long (wyland_long)", 
    0, NULL, NULL, 0, 0
  );

  wyland_throw(&error);

  return -1;
}

wyland_ulong wyland_flags_extract_ulong(wyland_flags *flags) {
  if (flags->fmt[flags->pos] == 'U') {
    if (flags->pos + 1 >= flags->len) {
      wylrterror error = wyland_make_error(
        "wyland_flags_extract_ulong(wyland_flags *)", 
        "out of range", "reading an unsigned long, but index is out of range", 
        0, NULL, NULL, 0, 0
      );
      wyland_throw(&error);
    }

    return *(wyland_ulong*)flags->objects[flags->pos++];
  }

  wylrterror error = wyland_make_error(
    "wyland_flags_extract_ulong(wyland_flags *)", 
    "invalid type convertion", "not an unsigned long (wyland_ulong)", 
    0, NULL, NULL, 0, 0
  );

  wyland_throw(&error);

  return -1;
}

wyland_char wyland_flags_extract_char(wyland_flags *flags) {
  if (flags->fmt[flags->pos] == 'c') {
    if (flags->pos + 1 >= flags->len) {
      wylrterror error = wyland_make_error(
        "wyland_flags_extract_char(wyland_flags *)", 
        "out of range", "reading an unsigned long, but index is out of range", 
        0, NULL, NULL, 0, 0
      );
      wyland_throw(&error);
    }

    return *(wyland_char*)flags->objects[flags->pos++];
  }

  wylrterror error = wyland_make_error(
    "wyland_flags_extract_char(wyland_flags *)", 
    "invalid type convertion", "not an unsigned long (wyland_ulong)", 
    0, NULL, NULL, 0, 0
  );

  wyland_throw(&error);

  return -1;
}

wyland_uchar wyland_flags_extract_uchar(wyland_flags *flags) {
  if (flags->fmt[flags->pos] == 'C') {
    if (flags->pos + 1 >= flags->len) {
      wylrterror error = wyland_make_error(
        "wyland_flags_extract_uchar(wyland_flags *)", 
        "out of range", "reading an unsigned long, but index is out of range", 
        0, NULL, NULL, 0, 0
      );
      wyland_throw(&error);
    }

    return *(wyland_uchar*)flags->objects[flags->pos++];
  }

  wylrterror error = wyland_make_error(
    "wyland_flags_extract_uchar(wyland_flags *)", 
    "invalid type convertion", "not an unsigned long (wyland_ulong)", 
    0, NULL, NULL, 0, 0
  );

  wyland_throw(&error);

  return -1;
}

float  wyland_get_runtime_version(void) {
  return 1.0006;
}