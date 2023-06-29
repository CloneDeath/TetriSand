#pragma once

#include <stdint.h>

struct tile_zone {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
};

struct tile_zone* new_tile_zone(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void delete_tile_zone(struct tile_zone* this);
