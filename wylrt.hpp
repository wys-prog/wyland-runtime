#pragma once

#include <string>
#include <sstream>
#include <iomanip> // For std::hex

#include "wylrt.h"

namespace wylma {
  namespace wyland {
    namespace runtime {

      class wyland_runtime_error : public wylrterror {
      private:
        wylrterror *object;

      public:
        // Default constructor
        wyland_runtime_error() : object(new wylrterror) {}

        // Constructor with parameters
        wyland_runtime_error(const char *what, const char *name, const char *caller, const char *exception_type,
                             uint64_t ip, uint64_t thread, uint64_t *segmbeg, uint64_t *segmend, uint64_t segmsize) {
          object = new wylrterror;
          object->what = strdup(what);
          object->name = strdup(name);
          object->caller = strdup(caller);
          object->exception_type = strdup(exception_type);
          object->ip = ip;
          object->thread = thread;
          object->segmbeg = segmbeg;
          object->segmend = segmend;
          object->segsize = segmsize;
        }

        wyland_runtime_error(const wylrterror &error) {
          object = new wylrterror(error);
        }

        // Copy constructor
        wyland_runtime_error(const wyland_runtime_error &other) {
          object = new wylrterror(*other.object);
        }

        // Move constructor
        wyland_runtime_error(wyland_runtime_error &&other) noexcept {
          object = other.object;
          other.object = nullptr;
        }

        // Destructor
        ~wyland_runtime_error() {
          delete object;
        }

        // Copy assignment operator
        wyland_runtime_error& operator=(const wyland_runtime_error &other) {
          if (this != &other) {
            delete object;
            object = new wylrterror(*other.object);
          }
          return *this;
        }

        // Move assignment operator
        wyland_runtime_error& operator=(wyland_runtime_error &&other) noexcept {
          if (this != &other) {
            delete object;
            object = other.object;
            other.object = nullptr;
          }
          return *this;
        }

        virtual const char *what() const {
          return object->what;
        }

        virtual const char *name() const {
          return object->name;
        }

        virtual const char *caller() const {
          return object->caller;
        }

        virtual const char *exception_name() const {
          return object->exception_type;
        }

        uint64_t ip() const {
          return object->ip;
        }

        uint64_t thread() const {
          return object->thread;
        }

        uint64_t *segmbeg() const {
          return object->segmbeg;
        }

        uint64_t *segmend() const {
          return object->segmend;
        }

        uint64_t segmsize() const {
          return object->segsize;
        }

        std::string fmterr(const char *on_new_line = "") const {
          std::stringstream ss;

          ss << name() << ": '" << what() << "'\n" << on_new_line 
             << "type:\t" << exception_name() << "\n" << on_new_line
             << "from:\t" << caller() << "\n" << on_new_line 
             << "ip:  \t" << std::hex << ip() << "\n" << on_new_line 
             << "flags:\tbeg(" << std::hex << reinterpret_cast<uintptr_t>(segmbeg()) << "), end(" << reinterpret_cast<uintptr_t>(segmend()) << ")\n" << on_new_line 
             << "segment size: " << segmsize() << std::endl;

          return ss.str();
        }
      };

      class wyland_invalid_argument : public wyland_runtime_error {
      public:
        wyland_invalid_argument(const char *what, const char *name, const char *caller,
                                uint64_t ip, uint64_t thread, uint64_t *segmbeg, uint64_t *segmend, uint64_t segmsize)
          : wyland_runtime_error(what, name, caller, typeid(this).name(), ip, thread, segmbeg, segmend, segmsize) {}
      };

      class wyland_out_of_range : public wyland_runtime_error {
      public:
        wyland_out_of_range(const char *what, const char *name, const char *caller,
                            uint64_t ip, uint64_t thread, uint64_t *segmbeg, uint64_t *segmend, uint64_t segmsize)
          : wyland_runtime_error(what, name, caller, typeid(this).name(), ip, thread, segmbeg, segmend, segmsize) {}
      };

      class wyland_logic_error : public wyland_runtime_error {
      public:
        wyland_logic_error(const char *what, const char *name, const char *caller,
                           uint64_t ip, uint64_t thread, uint64_t *segmbeg, uint64_t *segmend, uint64_t segmsize)
          : wyland_runtime_error(what, name, caller, typeid(this).name(), ip, thread, segmbeg, segmend, segmsize) {}
      };

      class wyland_runtime_warning : public wyland_runtime_error {
      public:
        wyland_runtime_warning(const char *what, const char *name, const char *caller,
                               uint64_t ip, uint64_t thread, uint64_t *segmbeg, uint64_t *segmend, uint64_t segmsize)
          : wyland_runtime_error(what, name, caller, typeid(this).name(), ip, thread, segmbeg, segmend, segmsize) {}
      };

      class wyland_invalid_pointer_exception : public wyland_runtime_error {
      public:
        wyland_invalid_pointer_exception(const char *what, const char *name, const char *caller,
                                         uint64_t ip, uint64_t thread, uint64_t *segmbeg, uint64_t *segmend, uint64_t segmsize)
          : wyland_runtime_error(what, name, caller, typeid(this).name(), ip, thread, segmbeg, segmend, segmsize) {}
      };
    }
  }
}