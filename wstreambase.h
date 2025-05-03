#ifndef ___WYLAND_STREAM_BASE___
#define ___WYLAND_STREAM_BASE___

#include "wylrt.h"

enum wstream_type {
  wstream_type_iostream, 
  wstream_type_ostream, 
  wstream_type_istream
};

typedef struct {
  void *stream; // DONT TUCH TOO
  enum wstream_type type; // DONT TUCH 
  wbool valid_ptr; // DONT TUCH
} wystream;

enum wstream_flags {
  failbit, 
  badbit,
  eofbit,
  goodbit
};
#ifdef __cplusplus
extern "C" {
#endif // ? C++

wystream                *wyland_new_ostringstream();
wystream                *wyland_new_istringstream();
wystream                *wyland_new_ofstream(const char*);
wystream                *wyland_new_ifstream(const char*);
void                     wyland_delete_stream(wystream*);
void                     wyland_flush_stream(wystream*);
void                     wyland_clear_stream(wystream*);
void                     wyland_set_flags_stream(wystream*, enum wstream_flags);
enum wstream_flags       wyland_get_flags_stream(wystream*);
wulong                   wyland_read_stream(wystream*, void *buffer, wulong size);
wulong                   wyland_write_stream(wystream*, const void *buffer, wulong size);
wint                     wyland_seekg_stream(wystream*, wlong offset, wlong origin);
wlong                    wyland_tellg_stream(wystream*);
wint                     wyland_seekp_stream(wystream*, wlong offset, wlong origin);
wlong                    wyland_tellp_stream(wystream*);
wint                     wyland_close_stream(wystream*);
wint                     wyland_open_stream(wystream*, const char*);
void                     wyland_stream_assign(wystream*, const wystream*);

#ifdef __cplusplus
}
#endif // ? C++
#endif // ? ___WYLAND_STREAM_BASE___