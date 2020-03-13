#ifndef _WRITER_EXPORTS_H_
#define _WRITER_EXPORTS_H_

#ifndef EXPORT_API
// #define EXPORT_API __attribute__ ((visibility("default")))
#define EXPORT_API
#endif 

extern "C" {

EXPORT_API void *new_writer();
EXPORT_API void delete_writer(void *writer_handle);
EXPORT_API int stateful_increment(void *writer_handle, int val);
EXPORT_API void stateful_write(void *writer_handle, const char *c);

}

#endif