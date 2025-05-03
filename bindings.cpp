#include "wylrt.h"
#include "wylrt.hpp"
#include "wstreambase.h"

#include <sstream>
#include <fstream>

namespace wylma { namespace wyland { namespace bindings {
  class bindings_exception : public runtime::wyland_runtime_error {
  public:
    bindings_exception(const std::string &what, const std::string &from) 
      : runtime::wyland_runtime_error(what.c_str(), "bindings exception", std::string("bindings/" + from).c_str(), typeid(this).name(), 0, 0, nullptr, nullptr, 0) {}
  };

} } }

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
        e.what(), "std::exception", "C++ Standard exception occurred", 0, nullptr, nullptr, 0, 0);
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

#pragma region bindings Wyland stream

  wystream *wyland_new_ostringstream() {
    wystream *mystream = new wystream();
    mystream->stream = (void*)new std::ostringstream();
    mystream->type  = wstream_type_ostream;
    mystream->valid_ptr = wyland_true;
    return mystream;
  }

  wystream *wyland_new_istringstream() {
    wystream *mystream = new wystream();
    mystream->stream = (void*)new std::istringstream();
    mystream->type  = wstream_type_istream;
    mystream->valid_ptr = wyland_true;
    return mystream;
  }

  wystream *wyland_new_ofstream(const char *path) {
    wystream *mystream = new wystream();
    mystream->type  = wstream_type_ostream;
    mystream->valid_ptr = wyland_true;

    std::ofstream *ofptr = new std::ofstream();
    ofptr->open(path);
    if (!ofptr->is_open()) {
      delete mystream;
      throw wylma::wyland::runtime::wyland_runtime_error(
        "file not openned", "invalid handle", __func__, "exception", 0, 0, nullptr, nullptr, 0 
      );
    }
    mystream->stream = (void*)ofptr;
    return mystream;
  }

  wystream *wyland_new_ifstream(const char *path) {
    wystream *mystream = new wystream();
    mystream->type  = wstream_type_istream;
    mystream->valid_ptr = wyland_true;

    std::ifstream *ofptr = new std::ifstream();
    ofptr->open(path);
    if (!ofptr->is_open()) {
      delete mystream;
      throw wylma::wyland::bindings::bindings_exception(
        "file not openned", (__func__)
      );
    }
    
    mystream->stream = (void*)ofptr;
    return mystream;
  }

  void wyland_delete_stream(wystream *ptr) {
    switch (ptr->type) {
      case wstream_type_istream:
        delete (std::istream*)ptr->stream;
        ptr->valid_ptr = wyland_false;
        break;
      case wstream_type_ostream:
        delete (std::ostream*)ptr->stream;
        ptr->valid_ptr = wyland_false;
        break;
      case wstream_type_iostream:
        delete (std::iostream*)ptr->stream;
        ptr->valid_ptr = wyland_false;
        break;
      default:
        throw wylma::wyland::bindings::bindings_exception(
          "(wystream*)ptr->type isn't iostream, istream nor ostream !", __func__
        );
    }
  }

  void wyland_flush_stream(wystream *stream) {
    switch (stream->type) {
      case wstream_type_iostream:
      *((std::iostream*)(stream->stream)) << std::flush;
      break;
      case wstream_type_ostream:
        *((std::ostream*)(stream->stream)) << std::flush;
        break;
      default:
        throw wylma::wyland::bindings::bindings_exception(
          "invalid stream type (can't flush on istream...)", __func__
        );
    }
  }

  void wyland_clear_stream(wystream *stream) {
    if (!stream || !stream->stream) return;

    switch (stream->type) {
      case wstream_type_istream:
        static_cast<std::istream*>(stream->stream)->clear();
        break;
      case wstream_type_ostream:
        static_cast<std::ostream*>(stream->stream)->clear();
        break;
      case wstream_type_iostream:
        static_cast<std::iostream*>(stream->stream)->clear();
        break;
    }
  }

  void wyland_set_flags_stream(wystream *stream, enum wstream_flags flags) {
    std::ios_base::iostate f;
    switch (flags) {
      case failbit: f = std::ios_base::failbit; break;
      case badbit:  f = std::ios_base::badbit;  break;
      case eofbit:  f = std::ios_base::eofbit;  break;
      case goodbit: f = std::ios_base::goodbit; break;
      default: return;
    }

    switch (stream->type) {
      case wstream_type_istream:
        static_cast<std::istream*>(stream->stream)->setstate(f);
        break;
      case wstream_type_ostream:
        static_cast<std::ostream*>(stream->stream)->setstate(f);
        break;
      case wstream_type_iostream:
        static_cast<std::iostream*>(stream->stream)->setstate(f);
        break;
    }
  }

  enum wstream_flags wyland_get_flags_stream(wystream *stream) {
    std::ios_base::iostate state = std::ios_base::goodbit;

    switch (stream->type) {
      case wstream_type_istream:
        state = static_cast<std::istream*>(stream->stream)->rdstate();
        break;
      case wstream_type_ostream:
        state = static_cast<std::ostream*>(stream->stream)->rdstate();
        break;
      case wstream_type_iostream:
        state = static_cast<std::iostream*>(stream->stream)->rdstate();
        break;
    }

    if (state & std::ios_base::failbit) return failbit;
    if (state & std::ios_base::badbit)  return badbit;
    if (state & std::ios_base::eofbit)  return eofbit;
    return goodbit;
  }

  wulong wyland_read_stream(wystream *stream, void *buffer, wulong size) {
    if (!stream || !buffer || stream->type == wstream_type_ostream) return 0;

    std::istream *in =
      (stream->type == wstream_type_iostream)
        ? static_cast<std::iostream*>(stream->stream)
        : static_cast<std::istream*>(stream->stream);

    in->read(static_cast<char*>(buffer), size);
    return static_cast<wulong>(in->gcount());
  }

  wulong wyland_write_stream(wystream *stream, const void *buffer, wulong size) {
    if (!stream || !buffer || stream->type == wstream_type_istream) return 0;

    std::ostream *out =
      (stream->type == wstream_type_iostream)
        ? static_cast<std::iostream*>(stream->stream)
        : static_cast<std::ostream*>(stream->stream);

    out->write(static_cast<const char*>(buffer), size);
    return size;
  }

  wint wyland_seekg_stream(wystream *stream, wlong offset, wlong origin) {
    if (!stream || stream->type == wstream_type_ostream) return -1;

    std::istream *in =
      (stream->type == wstream_type_iostream)
        ? static_cast<std::iostream*>(stream->stream)
        : static_cast<std::istream*>(stream->stream);

    std::ios_base::seekdir dir = (origin == 0) ? std::ios_base::beg :
                                 (origin == 1) ? std::ios_base::cur :
                                                 std::ios_base::end;
    in->seekg(offset, dir);
    return in->fail() ? -1 : 0;
  }

  wlong wyland_tellg_stream(wystream *stream) {
    if (!stream || stream->type == wstream_type_ostream) return -1;

    std::istream *in =
      (stream->type == wstream_type_iostream)
        ? static_cast<std::iostream*>(stream->stream)
        : static_cast<std::istream*>(stream->stream);

    return static_cast<wlong>(in->tellg());
  }

  wint wyland_seekp_stream(wystream *stream, wlong offset, wlong origin) {
    if (!stream || stream->type == wstream_type_istream) return -1;

    std::ostream *out =
      (stream->type == wstream_type_iostream)
        ? static_cast<std::iostream*>(stream->stream)
        : static_cast<std::ostream*>(stream->stream);

    std::ios_base::seekdir dir = (origin == 0) ? std::ios_base::beg :
                                 (origin == 1) ? std::ios_base::cur :
                                                 std::ios_base::end;
    out->seekp(offset, dir);
    return out->fail() ? -1 : 0;
  }

  wlong wyland_tellp_stream(wystream *stream) {
    if (!stream || stream->type == wstream_type_istream) return -1;

    std::ostream *out =
      (stream->type == wstream_type_iostream)
        ? static_cast<std::iostream*>(stream->stream)
        : static_cast<std::ostream*>(stream->stream);

    return static_cast<wlong>(out->tellp());
  }

  wint wyland_close_stream(wystream *stream) {
    if (!stream || !stream->stream) return -1;

    if (stream->type == wstream_type_istream) {
      auto* in = dynamic_cast<std::ifstream*>(static_cast<std::istream*>(stream->stream));
      if (in) in->close();
      return 0;
    }

    if (stream->type == wstream_type_ostream) {
      auto* out = dynamic_cast<std::ofstream*>(static_cast<std::ostream*>(stream->stream));
      if (out) out->close();
      return 0;
    }

    if (stream->type == wstream_type_iostream) {
      auto* io = dynamic_cast<std::fstream*>(static_cast<std::iostream*>(stream->stream));
      if (io) io->close();
      return 0;
    }

    return -1;
  }

  wint wyland_open_stream(wystream *stream, const char *path) {
    if (!stream || !path) return -1;

    if (stream->type == wstream_type_istream) {
      auto* in = dynamic_cast<std::ifstream*>(static_cast<std::istream*>(stream->stream));
      if (in) {
        in->open(path);
        return in->is_open() ? 0 : -1;
      }
    }

    if (stream->type == wstream_type_ostream) {
      auto* out = dynamic_cast<std::ofstream*>(static_cast<std::ostream*>(stream->stream));
      if (out) {
        out->open(path);
        return out->is_open() ? 0 : -1;
      }
    }

    if (stream->type == wstream_type_iostream) {
      auto* io = dynamic_cast<std::fstream*>(static_cast<std::iostream*>(stream->stream));
      if (io) {
        io->open(path, std::ios::in | std::ios::out);
        return io->is_open() ? 0 : -1;
      }
    }

    return -1;
  }

  void wyland_stream_assign(wystream* target, const wystream* source) {
    if (!target || !source) return;
    target->stream = source->stream;
    target->type = source->type;
    target->valid_ptr = source->valid_ptr;
  }


#pragma endregion
}