#ifndef ___WYLAND_FLOATS_H___
#define ___WYLAND_FLOATS_H___

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define WYLAND_FLOAT_MAX_PRECISION 10000

typedef struct {
  int16_t  integer;
  uint16_t floating; // [0, WYLAND_FLOAT_MAX_PRECISION[
} wfloat;

static inline void wfloat_normalize(wfloat* wf) {
  if (wf->floating >= WYLAND_FLOAT_MAX_PRECISION) {
    wf->integer += wf->floating / WYLAND_FLOAT_MAX_PRECISION;
    wf->floating %= WYLAND_FLOAT_MAX_PRECISION;
  }

  if (wf->integer < 0 && wf->floating > 0) {
    wf->integer += 1;
    wf->floating = WYLAND_FLOAT_MAX_PRECISION - wf->floating;
  }
}

static inline int32_t wfloat_to_fixed(const wfloat wf) {
  return wf.integer * WYLAND_FLOAT_MAX_PRECISION + wf.floating;
}

static inline wfloat fixed_to_wfloat(int32_t val) {
  wfloat wf;
  wf.integer = val / WYLAND_FLOAT_MAX_PRECISION;
  wf.floating = val % WYLAND_FLOAT_MAX_PRECISION;
  wfloat_normalize(&wf);
  return wf;
}

static inline wfloat wfloat_add(wfloat a, wfloat b) {
  wfloat result;
  result.integer = a.integer + b.integer;
  result.floating = a.floating + b.floating;
  wfloat_normalize(&result);
  return result;
}

static inline wfloat wfloat_sub(wfloat a, wfloat b) {
  wfloat result;
  result.integer = a.integer - b.integer;
  if (a.floating >= b.floating) {
    result.floating = a.floating - b.floating;
  } else {
    result.integer -= 1;
    result.floating = (a.floating + WYLAND_FLOAT_MAX_PRECISION) - b.floating;
  }
  wfloat_normalize(&result);
  return result;
}

// Multiplication
static inline wfloat wfloat_mul(wfloat a, wfloat b) {
  int32_t fixed_a = wfloat_to_fixed(a);
  int32_t fixed_b = wfloat_to_fixed(b);
  int64_t product = (int64_t)fixed_a * (int64_t)fixed_b;
  // Diviser par la précision pour corriger l’échelle
  return fixed_to_wfloat((int32_t)(product / WYLAND_FLOAT_MAX_PRECISION));
}

static inline wfloat wfloat_div(wfloat a, wfloat b) {
  int32_t fixed_a = wfloat_to_fixed(a);
  int32_t fixed_b = wfloat_to_fixed(b);
  if (fixed_b == 0) {
    wfloat zero = {0, 0};
    return zero;
  }

  int64_t result = ((int64_t)fixed_a * WYLAND_FLOAT_MAX_PRECISION) / fixed_b;
  return fixed_to_wfloat((int32_t)result);
}

static inline wfloat wfloat_mod(wfloat a, wfloat b) {
  int32_t fixed_a = wfloat_to_fixed(a);
  int32_t fixed_b = wfloat_to_fixed(b);
  if (fixed_b == 0) {
    wfloat zero = {0, 0};
    return zero;
  }

  int32_t result = fixed_a % fixed_b;
  return fixed_to_wfloat(result);
}

static inline float wfloat_to_float(wfloat wf) {
  return wf.integer + (float)wf.floating / WYLAND_FLOAT_MAX_PRECISION;
}

static inline wfloat float_to_wfloat(float f) {
  wfloat wf;
  wf.integer = (int16_t)f;
  wf.floating = (uint16_t)((f - wf.integer) * WYLAND_FLOAT_MAX_PRECISION);
  wfloat_normalize(&wf);
  return wf;
}

static inline char *wfloat_to_str(wfloat wf) {
  static char buffer[32];
  snprintf(buffer, sizeof(buffer), "%d.%04u", wf.integer, wf.floating);
  return buffer;
}

static inline wfloat str_to_wfloat(const char *str) {
  wfloat wf = {0, 0};
  if (str == NULL || strlen(str) == 0) {
    return wf;
  }

  char *dot = (char*)strchr(str, '.');
  if (dot) {
    *dot = '\0'; // Temporarily split the string
    wf.integer = (int16_t)atoi(str);
    wf.floating = (uint16_t)(atoi(dot + 1) * WYLAND_FLOAT_MAX_PRECISION / (wint)pow(10, strlen(dot + 1)));
    *dot = '.'; // Restore the string
  } else {
    wf.integer = (int16_t)atoi(str);
  }

  wfloat_normalize(&wf);
  return wf;
}

#endif // ___WYLAND_FLOATS_H___
