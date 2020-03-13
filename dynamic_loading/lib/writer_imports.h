#ifndef _WRITER_IMPORTS_H_
#define _WRITER_IMPORTS_H_

using new_writer_f = void *();
using delete_writer_f = void(void *);
using stateful_increment_f = int(void *, int );
using stateful_write_f = void(void *, const char *);

#endif