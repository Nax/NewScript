#ifndef NEWSCRIPT_H
#define NEWSCRIPT_H

#include <stddef.h>

#if defined(__cplusplus)
# define NEWSCRIPT_API  extern "C"
#else
# define NEWSCRIPT_API
#endif

void nsParse(const char* data, size_t len);

#endif
