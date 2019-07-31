#ifndef SOURCE_LOC_H
#define SOURCE_LOC_H 1

#include "shstr.h"

typedef struct
{
    int line;
    int col;
    int len;
    ShString* filename;
} SourceLoc;

inline static void nsSourceLocSet(SourceLoc* sl, int line, int col, int len, ShString* filename)
{
    sl->line = line;
    sl->col = col;
    sl->len = len;
    sl->filename = nsCopyShString(filename);
}

inline static void nsSourceLocCopy()

#endif
