#include "TileBorder.h"
#include "tile_set.h"
#include "../res/tile-border.h"
#include <stdlib.h>
#include <gb/gb.h>

inline static void _initialize_border_tiles(struct TileBorder* this) {
    this->border_tiles = alloc_tile_set(9);

    tile_set__set_data(this->border_tiles, TILE_Border);

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

static struct TileBorder* new(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    struct TileBorder* tb = malloc(sizeof(struct TileBorder));
    tb->_initialize_border_tiles = &_initialize_border_tiles;

    tb->x = x;
    tb->y = y;
    tb->width = width;
    tb->height = height;
    tb->_initialize_border_tiles(tb);
    return tb;
}

static void delete(struct TileBorder* this) {
    free_tile_set(this->border_tiles);
    free(this);
}

const struct TileBorderClass TileBorder = {
    .new=&new,
    .delete=&delete
};


