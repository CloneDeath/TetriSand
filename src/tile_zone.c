#include "tile_zone.h"
#include "../res/tile_border.h"
#include <stdlib.h>
#include <stdbool.h>
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

    tz->sand_chains = calloc((tz->width - 2) * 8, sizeof(struct sand_chain));

    return tz;
}

void delete_tile_zone(struct tile_zone* this) {
    free_tile_set(this->inner_tiles);
    free_tile_set(this->border_tiles);
    free(this);
}

static uint8_t tile_data[16];
static uint8_t current_tile_index = 255; // Invalid tile index
static bool needs_update = false;

static inline void load_tile_into_cache(uint8_t tile_index) {
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

static inline void save_and_clear_cache() {
    if (needs_update && current_tile_index != 255) {
        set_bkg_data(current_tile_index, 1, tile_data);
    }
    current_tile_index = 255;
    needs_update = false;
}

static inline uint8_t get_bit(uint8_t tile_index, uint8_t x, uint8_t y) {
    load_tile_into_cache(tile_index);

    uint8_t rowL = tile_data[y * 2 + 0];
    uint8_t rowH = tile_data[y * 2 + 1];

    // Extract the bits corresponding to the pixel at position (x, y)
    uint8_t lowerBit = (rowL >> (7 - x)) & 1;
    uint8_t upperBit = (rowH >> (7 - x)) & 1;

    // Combine the bits to get the color value
    uint8_t value = (upperBit << 1) | lowerBit;

    return value;
}

static inline void set_bit(uint8_t tile_index, uint8_t x, uint8_t y, uint8_t value) {
    load_tile_into_cache(tile_index);

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

    needs_update = true;
}

static inline uint8_t get_sand(struct tile_zone* this, uint8_t x, uint8_t y) {
    uint8_t y_flipped = ((this->height - 2)*8 - 1) - y;

    uint8_t tile_width = this->width - 2;
    uint8_t tile_x = x / 8;
    uint8_t tile_y = y_flipped / 8;
    uint8_t tile_offset = tile_width * tile_y + tile_x;
    uint8_t tile_index = this->inner_tiles->start + tile_offset;
    uint8_t pixel_x = x % 8;
    uint8_t pixel_y = y_flipped % 8;
    return get_bit(tile_index, pixel_x, pixel_y);
}

static inline void set_sand(struct tile_zone* this, uint8_t x, uint8_t y, uint8_t value) {
    uint8_t y_flipped = ((this->height - 2)*8 - 1) - y;

    uint8_t tile_width = this->width - 2;
    uint8_t tile_x = x / 8;
    uint8_t tile_y = y_flipped / 8;
    uint8_t tile_offset = tile_width * tile_y + tile_x;
    uint8_t tile_index = this->inner_tiles->start + tile_offset;
    uint8_t pixel_x = x % 8;
    uint8_t pixel_y = y_flipped % 8;
    set_bit(tile_index, pixel_x, pixel_y, value);
}

static inline void _move_chain_down(struct tile_zone* this, uint8_t x, struct sand_chain* chain) {
    chain->y -= 1;
    set_sand(this, x, chain->y, chain->value);
    set_sand(this, x, chain->y + chain->length, DMG_WHITE);
}

static inline void _slide_sand_chain(struct tile_zone* this, uint8_t x, struct sand_chain* dest) {
    uint8_t target_y = dest->y + dest->length;
    struct sand_chain* current = &this->sand_chains[x];
    if (current->length == 0) return;
    if (current->y == 0 && current->length - 1 > target_y) {
        uint8_t amount_to_stay = target_y + 1;
        struct sand_chain* split = sand_chain__split(current, amount_to_stay);
        struct sand_chain* split_end = sand_chain__get_last_connected(split);
        current->next = split_end->next;
        split_end->next = dest->next;
        dest->next = split;
        split->y = target_y;
        return;
    }
}

static inline void _slide_sand(struct tile_zone* this, uint8_t x, struct sand_chain* dest) {
    if (x-1 >= 0) {
        _slide_sand_chain(this, x - 1, dest);
    }
    uint8_t width = (this->width-2) * 8;
    if (x+1 < width) {
        _slide_sand_chain(this, x + 1, dest);
    }
}

static inline void _collapse_empty_and_similar_chains(struct tile_zone* this) {
    uint8_t width = (this->width - 2) * 8;
    for (uint8_t x = 0; x < width; x++) {
        struct sand_chain *current = &this->sand_chains[x];

        while (current->next != NULL) {
            struct sand_chain *next = current->next;
            if (current->length == 0) {
                current->y = next->y;
                current->length = next->length;
                current->value = next->value;
                current->next = next->next;
                continue;
            }
            if (current->value == next->value && current->y + current->length == next->y) {
                current->length += next->length;
                current->next = next->next;
                next->next = NULL;
                free_sand_chain(next);
            }
            current = current->next;
        }
    }
}

void update_sand(struct tile_zone* this) {
    _collapse_empty_and_similar_chains(this);

    uint8_t width = (this->width - 2) * 8;

    for (uint8_t x = 0; x < width; x++) {
        struct sand_chain *chain = &this->sand_chains[x];
        if (chain->length == 0) {
            _slide_sand(this, x, chain);
            continue;
        }
        if (chain->y > 0) {
            _move_chain_down(this, x, chain);
        } else {
            _slide_sand(this, x, chain);
        }

        struct sand_chain *prev_chain = chain;
        chain = chain->next;
        while (chain) {
            if (chain->y > prev_chain->y + prev_chain->length) {
                _move_chain_down(this, x, chain);
            }
            prev_chain = chain;
            chain = chain->next;
        }
    }

    save_and_clear_cache();
}

void add_sand(struct tile_zone* this, uint8_t x, uint8_t y, uint8_t length, uint8_t value) {
    struct sand_chain* chain = &this->sand_chains[x];
    struct sand_chain* new_chain = sand_chain__add_chain(chain, y, length, value);

    for (uint8_t y_at = new_chain->y; y_at < new_chain->y + new_chain->length; y_at++){
        set_sand(this, x, y_at, value);
    }
    save_and_clear_cache();
}
