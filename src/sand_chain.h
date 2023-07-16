#pragma once

#include <stdint.h>
#include <stdlib.h>

struct sand_chain {
    uint8_t y;
    uint8_t length;
    uint8_t value;
    struct sand_chain* next;
};

struct sand_chain* new_sand_chain(uint8_t y, uint8_t length, uint8_t value);
struct sand_chain* copy_sand_chain(struct sand_chain* copy);
void free_sand_chain(struct sand_chain* this);
struct sand_chain* sand_chain__add_chain(struct sand_chain* this, uint8_t y, uint8_t length, uint8_t value);
struct sand_chain* sand_chain__get_last_connected(struct sand_chain* this);
uint8_t sand_chain__get_gap_above(struct sand_chain* this);
uint8_t sand_chain__get_connected_length(struct sand_chain* this);
struct sand_chain* sand_chain__excise_chain(struct sand_chain* this, uint8_t from, uint8_t length);
void sand_chain__try_to_combine(struct sand_chain *this);

extern const struct SandChainClass {
    struct sand_chain* (*new)(uint8_t y, uint8_t length, uint8_t value);
    struct sand_chain* (*new_array)(size_t count);
    struct sand_chain* (*copy)(struct sand_chain* original);
    void (*delete)(struct sand_chain* this);
} SandChain;