#include "wylrt.h"
#include "wylrt.hpp"

extern "C" {

  void wyland_throw(const wylrterror *error) {
    wylma::wyland::runtime::wyland_runtime_error err(*error);
    throw err;
  }

  wint wyland_try_callable(wyland_callable *callable, wyland_flags *flags, wyland_error_block *errblck) {
    if (!errblck) throw wylma::wyland::runtime::wyland_invalid_pointer_exception(
        "errblck is not a valid pointer", "pointer exception", 
        __func__, 0, 0, nullptr, nullptr, -1
      );
    
    try {
      wyland_int result = (*callable)(flags); // Call the callable
      return result;
    } catch (const std::exception &e) {
      wylrterror error = wyland_make_error(
        e.what(), "std::exception", "Standard exception occurred", 0, nullptr, nullptr, 0, 0);
      (*errblck)(&error);
    } catch (const wylma::wyland::runtime::wyland_runtime_error &e) {
      wylrterror error = wyland_make_error(
        e.what(), "wyland_runtime_error", "Wyland runtime error occurred", 0, nullptr, nullptr, 0, 0);
      (*errblck)(&error);
    } catch (const wylrterror &e) {
        (*errblck)(&e);
    } catch (...) {
      wylrterror error = wyland_make_error(
        "Unknown error", "unknown", "An unknown error occurred", 0, nullptr, nullptr, 0, 0);
      (*errblck)(&error);
    }

    return -1;
  }
}