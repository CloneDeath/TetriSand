#include "sand_memory.h"
#include "sand_chain.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>

#include <gb/gb.h>

/******* PRIVATE CLASS MEMBERS *******/

volatile uint8_t* const RAM = 0xA000;

volatile sand_chain* const sand_start = (sand_chain*)0xA000;
volatile sand_chain* const sand_end = (sand_chain*)(0xA000 + 0x2000);

sand_chain* first_free = NULL;

/******* PRIVATE CLASS FUNCTIONS *******/

inline static bool __is_free(sand_chain* chain) {
    return chain->is_free;
}
inline static bool __is_used(sand_chain* chain) {
    return !__is_free(chain);
}
inline static void __set_free(sand_chain* chain) {
    chain->is_free = 1;
}
inline static void __set_used(sand_chain* chain) {
    chain->is_free = 0;
}
inline static void __initialize_array(sand_chain* start, size_t count) {
    for (size_t i = 0; i < count; i++) {
        __set_used(start+i);
        (start+i)->y = 0;
        (start+i)->length = 0;
        (start+i)->value = 0;
        (start+i)->family = 0;
        (start+i)->next = NULL;
    }
}

/******* CLASS *******/

void __init_memory() {
    first_free = (sand_chain*)sand_start;
    for (sand_chain* c = (sand_chain*)sand_start; c < sand_end; c++) {
        __set_free(c);
    }
}

sand_chain* sand_memory__alloc() {
    if (first_free == NULL) {
        __init_memory();
    }
    sand_chain* current = first_free;
    while (current < sand_end) {
        if (__is_free(current)) {
            first_free = current+1;
            __set_used(current);
            return current;
        }
        current++;
    }

    // ran out of memory, start over, looking for free slots
    current = (sand_chain*)sand_start;
    while (current < sand_end) {
        if (__is_free(current)) {
            first_free = current+1;
            __set_used(current);
            return current;
        }
        current++;
    }

    HIDE_SPRITES;
    printf("FATAL - Out of Dedicated Sand Memory.");
    exit(-1);
    return NULL;
}

sand_chain* sand_memory__calloc(size_t count) {
    if (first_free == NULL) {
        __init_memory();
    }
    sand_chain* start = first_free;
    sand_chain* current = first_free;
    while (current < sand_end) {
        if (__is_used(current)) {
            start = current + 1;
        }
        current++;

        if (current - start >= count) {
            first_free = current;
            __initialize_array(start, count);
            return start;
        }
    }

    // ran out of memory, start over, looking for free slots
    start = (sand_chain*)sand_start;
    current = (sand_chain*)sand_start;
    while (current < sand_end) {
        if (__is_used(current)) {
            start = current + 1;
        }
        current++;

        if (current - start >= count) {
            first_free = current;
            __initialize_array(start, count);
            return start;
        }
    }

    HIDE_SPRITES;
    printf("FATAL - Out of Dedicated Sand Memory.");
    exit(-1);
    return NULL;
}

void sand_memory__free(sand_chain* pointer) {
    assert(__is_used(pointer));
    if (pointer < first_free) first_free = pointer;
    __set_free(pointer);
}