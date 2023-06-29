#include "tile_zone.h"
#include "../res/tile_border.h"
#include "stdlib.h"
#include <gb/gb.h>

struct tile_zone* new_tile_zone(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    struct tile_zone* tz = (struct tile_zone*)malloc(sizeof(struct tile_zone));
    tz->x = x;
    tz->y = y;
    tz->width = width;
    tz->height = height;

    set_bkg_data(1, 9, TileBorder);

    set_bkg_tile_xy(x, y, 1); // Top-Left
    set_bkg_tile_xy(x + width - 1, y, 3); // Top-Right
    set_bkg_tile_xy(x, y + height - 1, 6); // Bottom-Left
    set_bkg_tile_xy(x + width - 1, y + height - 1, 8); // Bottom-Right

    for (uint8_t i = 1; i < width-1; i++){
        set_bkg_tile_xy(x + i, y, 2); // Top
        set_bkg_tile_xy(x + i, y + height - 1, 7); // Bottom
    }
    for (uint8_t i = 1; i < height-1; i++){
        set_bkg_tile_xy(x, y + i, 4); // Left
        set_bkg_tile_xy(x + width - 1, y + i, 5); // Right
    }

    return tz;
}