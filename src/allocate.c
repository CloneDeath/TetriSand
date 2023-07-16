#include "allocate.h"
#include <stdlib.h>
#include <stdio.h>
#include <gb/gb.h>

void* allocate(size_t size) {
    void* data = malloc(size);
    if (data == NULL) {
        HIDE_SPRITES;
        printf("OUT OF MEMORY\n");
        printf("allocate 0x%x", size);
        exit(-1);
    }
    return data;
}

void* allocate_array(size_t count, size_t size) {
    void* data = calloc(count, size);
    if (data == NULL) {
        HIDE_SPRITES;
        printf("OUT OF MEMORY\n");
        printf("allocate_array %uh x 0x%x", count, size);
        exit(-1);
    }
    return data;
}