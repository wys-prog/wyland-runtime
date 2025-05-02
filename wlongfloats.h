#ifndef ___WLONGLFLOAT_H___
#define ___WLONGLFLOAT_H___

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "wylrt.h"

#define WLONGFLOAT_MAX_PRECISION 1000000000ULL // Woaw

typedef struct {
  int64_t  integer;
  uint64_t floating; // [0, WLONGFLOAT_MAX_PRECISION[
} wlongfloat;

static inline void longfloat_normalize(wlongfloat* lf) {
  if (lf->floating >= WLONGFLOAT_MAX_PRECISION) {
    lf->integer += lf->floating / WLONGFLOAT_MAX_PRECISION;
    lf->floating %= WLONGFLOAT_MAX_PRECISION;
  }

  if (lf->integer < 0 && lf->floating > 0) {
    lf->integer += 1;
    lf->floating = WLONGFLOAT_MAX_PRECISION - lf->floating;
  }
}

static inline int64_t longfloat_to_fixed(const wlongfloat lf) {
  return lf.integer * WLONGFLOAT_MAX_PRECISION + lf.floating;
}

static inline wlongfloat fixed_to_longfloat(int64_t val) {
  wlongfloat lf;
  lf.integer = val / WLONGFLOAT_MAX_PRECISION;
  lf.floating = val % WLONGFLOAT_MAX_PRECISION;
  longfloat_normalize(&lf);
  return lf;
}

static inline wlongfloat longfloat_add(wlongfloat a, wlongfloat b) {
  wlongfloat result;
  result.integer = a.integer + b.integer;
  result.floating = a.floating + b.floating;
  longfloat_normalize(&result);
  return result;
}

static inline wlongfloat longfloat_sub(wlongfloat a, wlongfloat b) {
  wlongfloat result;
  result.integer = a.integer - b.integer;
  if (a.floating >= b.floating) {
    result.floating = a.floating - b.floating;
  } else {
    result.integer -= 1;
    result.floating = (a.floating + WLONGFLOAT_MAX_PRECISION) - b.floating;
  }
  longfloat_normalize(&result);
  return result;
}

static inline wlongfloat longfloat_mul(wlongfloat a, wlongfloat b) {
  int64_t fixed_a = longfloat_to_fixed(a);
  int64_t fixed_b = longfloat_to_fixed(b);
  __int128 product = (__int128)fixed_a * (__int128)fixed_b;
  return fixed_to_longfloat((int64_t)(product / WLONGFLOAT_MAX_PRECISION));
}

static inline wlongfloat longfloat_div(wlongfloat a, wlongfloat b) {
  int64_t fixed_a = longfloat_to_fixed(a);
  int64_t fixed_b = longfloat_to_fixed(b);
  if (fixed_b == 0) {
    wlongfloat zero = {0, 0};
    return zero;
  }

  __int128 result = ((__int128)fixed_a * WLONGFLOAT_MAX_PRECISION) / fixed_b;
  return fixed_to_longfloat((int64_t)result);
}

static inline wlongfloat longfloat_mod(wlongfloat a, wlongfloat b) {
  int64_t fixed_a = longfloat_to_fixed(a);
  int64_t fixed_b = longfloat_to_fixed(b);
  if (fixed_b == 0) {
    wlongfloat zero = {0, 0};
    return zero;
  }

  int64_t result = fixed_a % fixed_b;
  return fixed_to_longfloat(result);
}

static inline double longfloat_to_double(wlongfloat lf) {
  return (double)lf.integer + (double)lf.floating / WLONGFLOAT_MAX_PRECISION;
}

static inline wlongfloat double_to_longfloat(double d) {
  wlongfloat lf;
  lf.integer = (int64_t)d;
  lf.floating = (uint64_t)((d - lf.integer) * WLONGFLOAT_MAX_PRECISION);
  longfloat_normalize(&lf);
  return lf;
}

static inline long double longfloat_to_long_double(wlongfloat lf) {
  return (long double)lf.integer + (long double)lf.floating / WLONGFLOAT_MAX_PRECISION;
}

static inline wlongfloat long_double_to_longfloat(long double ld) {
  wlongfloat lf;
  lf.integer = (int64_t)ld;
  lf.floating = (uint64_t)((ld - lf.integer) * WLONGFLOAT_MAX_PRECISION);
  longfloat_normalize(&lf);
  return lf;
}

static inline char *longfloat_to_str(wlongfloat lf) {
  static char buffer[64];
  snprintf(buffer, sizeof(buffer), "%lld.%09llu", lf.integer, lf.floating);
  return buffer;
}

static inline wlongfloat str_to_longfloat(const char *str) {
  wlongfloat wf = {0, 0};
  if (str == NULL || strlen(str) == 0) {
    return wf;
  }

  char *dot = (char*)strchr(str, '.');
  if (dot) {
    *dot = '\0'; // Temporarily split the string
    wf.integer = (int64_t)atoll(str);
    wf.floating = (uint64_t)(atoll(dot + 1) * WLONGFLOAT_MAX_PRECISION / (int)pow(10, strlen(dot + 1)));
    *dot = '.'; // Restore the string
  } else {
    wf.integer = (int16_t)atoi(str);
  }

  longfloat_normalize(&wf);
  return wf;
}

#endif // ___WLONGLFLOAT_H___
