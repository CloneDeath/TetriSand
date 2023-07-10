#pragma once

#include <stdint.h>

struct tile_border {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    struct tile_set* border_tiles;
};

struct tile_border* new_tile_border(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void delete_tile_border(struct tile_border* this);