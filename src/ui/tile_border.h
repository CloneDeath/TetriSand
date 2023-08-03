#pragma once

#include <stdint.h>
#include "../engine/tile_set.h"

typedef struct tile_border_struct {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    tile_set* border_tiles;
} tile_border;

/******* CLASS *******/
tile_border* tile_border__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void tile_border__delete(tile_border* this);