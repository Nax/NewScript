#include <stdlib.h>
#include <stdio.h>
#include <newscript/newscript.h>

int main(int argc, char** argv)
{
    FILE* file;
    size_t len;
    char* buf;

    file = fopen(argv[1], "r");
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);
    buf = malloc(len);
    fread(buf, len, 1, file);
    fclose(file);
    nsParse(buf, len);
    free(buf);
    return 0;
}
