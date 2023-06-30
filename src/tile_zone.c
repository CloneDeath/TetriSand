#include "tile_zone.h"
#include "../res/tile_border.h"
#include <stdlib.h>
#include <gb/gb.h>
#include "tile_set.h"

void initialize_border_tiles(struct tile_zone* this) {
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

void initialize_inner_tiles(struct tile_zone* this) {
    uint8_t inner_width = this->width - 2;
    uint8_t inner_height = this->height - 2;
    uint8_t nb_tiles = inner_width * inner_height;
    this->inner_tiles = alloc_tile_set(nb_tiles);

    for (uint16_t z = 0; z < nb_tiles; z++) {
        const uint8_t zero[] = {
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };
        set_bkg_data(this->inner_tiles->start + z, 1, zero);
    }


    for (uint8_t x = 0; x < inner_width; x++) {
        for (uint8_t y = 0; y < inner_height; y++) {
            uint8_t tile_offset = y * inner_width + x;
            set_bkg_tile_xy(this->x + x + 1, this->y + y + 1, this->inner_tiles->start + tile_offset);
        }
    }
}

struct tile_zone* new_tile_zone(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    struct tile_zone* tz = (struct tile_zone*)malloc(sizeof(struct tile_zone));
    tz->x = x;
    tz->y = y;
    tz->width = width;
    tz->height = height;

    initialize_border_tiles(tz);
    initialize_inner_tiles(tz);

    return tz;
}

void delete_tile_zone(struct tile_zone* this) {
    free_tile_set(this->inner_tiles);
    free_tile_set(this->border_tiles);
    free(this);
}

void set_bit(uint8_t tile_index, uint8_t x, uint8_t y, uint8_t value) {
    static uint8_t tile_data[16];
    get_bkg_data(tile_index, 1, tile_data);
    uint8_t rowL = tile_data[y * 2 + 0];
    uint8_t rowH = tile_data[y * 2 + 1];

    // Limit value to two bits (0-3)
    value &= 3;

    // Clear the bits
    rowL &= ~(1 << (7 - x));
    rowH &= ~(1 << (7 - x));

    // Set the new bits
    rowL |= (value & 1) << (7 - x);
    rowH |= (value >> 1) << (7 - x);

    // Store the modified bytes back into the tile data
    tile_data[y * 2 + 0] = rowL;
    tile_data[y * 2 + 1] = rowH;

    set_bkg_data(tile_index, 1, tile_data);
}

void set_sand(struct tile_zone* this, uint8_t x, uint8_t y, uint8_t value) {
    uint8_t tile_width = this->width - 2;
    uint8_t tile_x = x / 8;
    uint8_t tile_y = y / 8;
    uint8_t tile_offset = tile_width * tile_y + tile_x;
    uint8_t tile_index = this->inner_tiles->start + tile_offset;
    uint8_t pixel_x = x % 8;
    uint8_t pixel_y = y % 8;
    set_bit(tile_index, pixel_x, pixel_y, value);
}