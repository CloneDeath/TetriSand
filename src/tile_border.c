#include "tile_border.h"
#include "tile_set.h"
#include "../res/border_tiles.h"
#include <stdlib.h>
#include <gb/gb.h>

inline static void __initialize_border_tiles(struct tile_border* this) {
    this->border_tiles = alloc_tile_set(9);

    set_bkg_data(this->border_tiles->start, this->border_tiles->count, TileBorder);

    set_bkg_tile_xy(this->x, this->y, this->border_tiles->start + 0); // Top-Left
    set_bkg_tile_xy(this->x + this->width - 1, this->y, this->border_tiles->start + 2); // Top-Right
    set_bkg_tile_xy(this->x, this->y + this->height - 1, this->border_tiles->start + 5); // Bottom-Left
    set_bkg_tile_xy(this->x + this->width - 1, this->y + this->height - 1, this->border_tiles->start + 7); // Bottom-Right

    for (uint8_t i = 1; i < this->width-1; i++){
        set_bkg_tile_xy(this->x + i, this->y, this->border_tiles->start + 1); // Top
        set_bkg_tile_xy(this->x + i, this->y + this->height - 1, this->border_tiles->start + 6); // Bottom
    }
    for (uint8_t i = 1; i < this->height-1; i++){
        set_bkg_tile_xy(this->x, this->y + i, this->border_tiles->start + 3); // Left
        set_bkg_tile_xy(this->x + this->width - 1, this->y + i, this->border_tiles->start + 4); // Right
    }
}

struct tile_border* new_tile_border(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    struct tile_border* tb = (struct tile_border*)malloc(sizeof(struct tile_border));
    tb->x = x;
    tb->y = y;
    tb->width = width;
    tb->height = height;
    __initialize_border_tiles(tb);
    return tb;
}

void delete_tile_border(struct tile_border* this) {
    free_tile_set(this->border_tiles);
    free(this);
}