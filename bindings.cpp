#include "wylrt.h"
#include "wylrt.hpp"

extern "C" {

  void wyland_throw(const wylrterror *error) {
    wylma::wyland::runtime::wyland_runtime_error err(*error);
    throw err;
  }

}