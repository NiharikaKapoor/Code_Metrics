#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <ctype.h>

void getComplexities(const char *code);

struct MemoryStruct {
    char *memory;
    size_t size;
};