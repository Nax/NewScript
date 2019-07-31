#ifndef SHSTR_H
#define SHSTR_H 1

typedef struct
{
    int count;
    char str[];
} ShString;

ShString*   nsCreateShString(const char* str);
ShString*   nsCopyShString(ShString* str);
void        nsDestroyShString(ShString* str);

#endif
