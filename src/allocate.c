#include "allocate.h"
#include <stdlib.h>
#include <stdio.h>
#include <gb/gb.h>

size_t total = 0;

void* allocate(size_t size) {
    total += size;

    void* data = malloc(size);
    if (data == NULL) {
        HIDE_SPRITES;
        printf("OUT OF MEMORY\n");
        printf("TOTAL: 0x%x\n", total);
        printf("allocate 0x%x", size);
        exit(-1);
    }
    return data;
}

void* allocate_array(size_t count, size_t size) {
    total += count * size;

    void* data = calloc(count, size);
    if (data == NULL) {
        HIDE_SPRITES;
        printf("OUT OF MEMORY\n");
        printf("TOTAL: 0x%x\n", total);
        printf("allocate_array %hu x 0x%x", count, size);
        exit(-1);
    }
    return data;
}