#pragma once

#include "stdint.h"

struct sand_chain {
    uint8_t y;
    uint8_t length;
    uint8_t value;
    struct sand_chain* next;
};

struct sand_chain* new_sand_chain(uint8_t y, uint8_t length, uint8_t value);
struct sand_chain* chain__add_chain(struct sand_chain* this, uint8_t y, uint8_t length, uint8_t value);