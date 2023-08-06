#pragma bank 255

#include "bitmap_area.h"
#include "allocate.h"

#include "../../res/TILES_blank.h"

#include <stdbool.h>
#include <stdio.h>

#include <gb/gb.h>

static uint8_t tile_data[16];
static uint8_t current_tile_index = 255; // Invalid tile index
static bool needs_update = false;

/******* PRIVATE CLASS *******/

static inline void _load_tile_colors_into_cache(uint8_t tile_index) {
    if (tile_index == current_tile_index) return;

    // Save the modified data of the current tile before loading new tile
    if (needs_update && current_tile_index != 255) {
        set_bkg_data(current_tile_index, 1, tile_data);
    }

    // Load the new tile data
    get_bkg_data(tile_index, 1, tile_data);
    current_tile_index = tile_index;
    needs_update = false;
}

void bitmap_area__flush_cache() BANKED {
    if (needs_update && current_tile_index != 255) {
        set_bkg_data(current_tile_index, 1, tile_data);
    }
    current_tile_index = 255;
    needs_update = false;
}

/******* PRIVATE INSTANCE *******/

static inline uint8_t _get_tile_color_bit(uint8_t tile_index, uint8_t x, uint8_t y) {
    _load_tile_colors_into_cache(tile_index);

    uint8_t rowL = tile_data[y * 2 + 0];
    uint8_t rowH = tile_data[y * 2 + 1];

    // Extract the bits corresponding to the pixel at position (x, y)
    uint8_t lowerBit = (rowL >> (7 - x)) & 1;
    uint8_t upperBit = (rowH >> (7 - x)) & 1;

    // Combine the bits to get the color value
    uint8_t value = (upperBit << 1) | lowerBit;

    return value;
}

static const uint8_t masks[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

static inline void _set_tile_color_bit(uint8_t tile_index, uint8_t x, uint8_t y, uint8_t value) {
    _load_tile_colors_into_cache(tile_index);

    uint8_t shift = 7 - x;

    uint8_t rowL = tile_data[y * 2 + 0];
    uint8_t rowH = tile_data[y * 2 + 1];

    // Limit value to two bits (0-3)
    value &= 3;

    // Clear the bits using precomputed masks
    rowL &= masks[x];
    rowH &= masks[x];

    // Set the new bits
    rowL |= (value & 1) << shift;
    rowH |= (value >> 1) << shift;

    // Store the modified bytes back into the tile data
    tile_data[y * 2 + 0] = rowL;
    tile_data[y * 2 + 1] = rowH;

    needs_update = true;
}

/******* PUBLIC INSTANCE *******/

uint8_t bitmap_area__get_color(bitmap_area* this, uint8_t x, uint8_t y) BANKED {
    uint8_t y_flipped = (this->height * 8 - 1) - y;

    uint8_t tile_x = x / 8;
    uint8_t tile_y = y_flipped / 8;
    uint8_t tile_offset = this->width * tile_y + tile_x;
    uint8_t tile_index = this->inner_tiles->start + tile_offset;
    uint8_t pixel_x = x % 8;
    uint8_t pixel_y = y_flipped % 8;
    return _get_tile_color_bit(tile_index, pixel_x, pixel_y);
}

void bitmap_area__set_color(bitmap_area* this, uint8_t x, uint8_t y, uint8_t value) BANKED {
    if (x > this->width * 8 || y > this->height * 8) {
        HIDE_SPRITES;
        printf("BOUNDS FAULT - x:%hu y:%hu", (uint8_t)x, (uint8_t)y);
        exit(-1);
    }

    uint8_t y_flipped = (this->height * 8 - 1) - y;

    uint8_t tile_x = x / 8;
    uint8_t tile_y = y_flipped / 8;
    uint8_t tile_offset = this->width * tile_y + tile_x;
    uint8_t tile_index = this->inner_tiles->start + tile_offset;
    uint8_t pixel_x = x % 8;
    uint8_t pixel_y = y_flipped % 8;

    if (tile_index < this->inner_tiles->start || tile_index > this->inner_tiles->start + this->inner_tiles->count) {
        HIDE_SPRITES;
        printf("INDEX FAULT - %hu %hu %hu", x, y, tile_index);
        exit(-1);
    }

    _set_tile_color_bit(tile_index, pixel_x, pixel_y, value);
}

/******* PRIVATE CLASS *******/

void __initialize_inner_tiles(bitmap_area* this) {
    uint8_t nb_tiles = this->width * this->height;
    this->inner_tiles = tile_set__alloc(nb_tiles);

    for (uint16_t z = 0; z < nb_tiles; z++) {
        set_bkg_data(this->inner_tiles->start + z, TILES_blankLen, TILES_blank);
    }

    for (uint8_t x = 0; x < this->width; x++) {
        for (uint8_t y = 0; y < this->height; y++) {
            uint8_t tile_offset = y * this->width + x;
            set_bkg_tile_xy(this->x + x, this->y + y, this->inner_tiles->start + tile_offset);
        }
    }
}

/******* PUBLIC CLASS *******/

bitmap_area* bitmap_area__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height) BANKED {
    bitmap_area* this = allocate(sizeof(bitmap_area));
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    __initialize_inner_tiles(this);

    return this;
}

void bitmap_area__delete(bitmap_area* this) BANKED {
    tile_set__free(this->inner_tiles);
    free(this);
}