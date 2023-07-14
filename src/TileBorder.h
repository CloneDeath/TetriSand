#pragma once

#include <stdint.h>

struct TileBorder {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    struct tile_set* border_tiles;

    // private:
    void (*_initialize_border_tiles)(struct TileBorder* this);
};

extern const struct TileBorderClass {
    struct TileBorder* (*new)(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void (*delete)(struct TileBorder* this);
} TileBorder;