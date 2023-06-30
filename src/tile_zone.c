#include "tile_zone.h"
#include "../res/tile_border.h"
#include "stdlib.h"
#include <gb/gb.h>
#include "tile_set.h"

struct tile_zone* new_tile_zone(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    struct tile_zone* tz = (struct tile_zone*)malloc(sizeof(struct tile_zone));
    tz->x = x;
    tz->y = y;
    tz->width = width;
    tz->height = height;

    struct tile_set* tiles = alloc_tile_set(9);

    set_bkg_data(tiles->start, tiles->count, TileBorder);

    set_bkg_tile_xy(x, y, tiles->start + 0); // Top-Left
    set_bkg_tile_xy(x + width - 1, y, tiles->start + 2); // Top-Right
    set_bkg_tile_xy(x, y + height - 1, tiles->start + 5); // Bottom-Left
    set_bkg_tile_xy(x + width - 1, y + height - 1, tiles->start + 7); // Bottom-Right

    for (uint8_t i = 1; i < width-1; i++){
        set_bkg_tile_xy(x + i, y, tiles->start + 1); // Top
        set_bkg_tile_xy(x + i, y + height - 1, tiles->start + 6); // Bottom
    }
    for (uint8_t i = 1; i < height-1; i++){
        set_bkg_tile_xy(x, y + i, tiles->start + 3); // Left
        set_bkg_tile_xy(x + width - 1, y + i, tiles->start + 4); // Right
    }

    return tz;
}

void delete_tile_zone(struct tile_zone* this) {
    free(this);
}