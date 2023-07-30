#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct sand_chain_struct {
    uint8_t y;
    uint8_t length;
    uint8_t value;
    struct sand_chain_struct* next;
} sand_chain;

/******* PUBLIC *******/

sand_chain* sand_chain__add_chain(sand_chain* this, uint8_t y, uint8_t length, uint8_t value);
sand_chain* sand_chain__get_last_connected(sand_chain* this);
uint8_t     sand_chain__get_gap_above(sand_chain* this);
uint8_t     sand_chain__get_connected_length(sand_chain* this);
sand_chain* sand_chain__excise_chain(sand_chain* this, uint8_t from, uint8_t length);
void        sand_chain__try_to_combine(sand_chain *this);
int8_t      sand_chain__get_adjacency(sand_chain* this, sand_chain* other);

/******* CLASS *******/

sand_chain* sand_chain__new(uint8_t y, uint8_t length, uint8_t value);
sand_chain* sand_chain__new_array(size_t count);
sand_chain* sand_chain__copy(sand_chain* original);
void        sand_chain__delete(sand_chain* this);