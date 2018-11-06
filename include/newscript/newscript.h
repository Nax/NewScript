#ifndef NEWSCRIPT_H
#define NEWSCRIPT_H

#include <stddef.h>

#if defined(__cplusplus)
# define NEWSCRIPT_API  extern "C"
#else
# define NEWSCRIPT_API
#endif

void nsDebugLex(const char* buffer, size_t length);

#endif