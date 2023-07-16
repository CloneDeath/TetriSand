#include "allocate.h"
#include <stdlib.h>
#include <stdio.h>

void* allocate(size_t size) {
    void* data = malloc(size);
    if (data == NULL) {
        printf("OUT OF MEMORY - allocate");
        exit(-1);
    }
    return data;
}

void* allocate_array(size_t count, size_t size) {
    void* data = calloc(count, size);
    if (data == NULL) {
        printf("OUT OF MEMORY - allocate_array");
        exit(-1);
    }
    return data;
}