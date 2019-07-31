#include <string.h>
#include <stdlib.h>
#include "shstr.h"

ShString* nsCreateShString(const char* str)
{
    ShString* shstr;

    shstr = malloc(sizeof(shstr) + strlen(str) + 1);
    shstr->count = 0;
    strcpy(&shstr->str, str);

    return shstr;
}

ShString* nsCopyShString(ShString* str)
{
    str->count++;
    return str;
}

void nsDestroyShString(ShString* str)
{
    if (str->count == 0)
        free(str);
    else
        str->count--;
}
