#pragma once

#include "stdint.h"

struct tile_set {
    uint8_t start;
    uint8_t count;
};

struct tile_set* alloc_tile_set(uint8_t nb_tiles);
void free_tile_set(struct tile_set* this);