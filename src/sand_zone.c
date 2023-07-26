#include "sand_zone.h"
#include "allocate.h"
#include "tile_set.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <gb/gb.h>

static uint8_t tile_data[16];
static uint8_t current_tile_index = 255; // Invalid tile index
static bool needs_update = false;

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

static inline void _save_and_clear_tile_colors_cache() {
    if (needs_update && current_tile_index != 255) {
        set_bkg_data(current_tile_index, 1, tile_data);
    }
    current_tile_index = 255;
    needs_update = false;
}

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

static inline void _set_tile_color_bit(uint8_t tile_index, uint8_t x, uint8_t y, uint8_t value) {
    _load_tile_colors_into_cache(tile_index);

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

static inline uint8_t _get_sand_color(sand_zone* this, uint8_t x, uint8_t y) {
    uint8_t y_flipped = (this->height * 8 - 1) - y;

    uint8_t tile_x = x / 8;
    uint8_t tile_y = y_flipped / 8;
    uint8_t tile_offset = this->width * tile_y + tile_x;
    uint8_t tile_index = this->inner_tiles->start + tile_offset;
    uint8_t pixel_x = x % 8;
    uint8_t pixel_y = y_flipped % 8;
    return _get_tile_color_bit(tile_index, pixel_x, pixel_y);
}

#include "stdio.h"
static inline void _set_sand_color(sand_zone* this, uint8_t x, uint8_t y, uint8_t value) {
    if (x > this->width * 8 || y > this->height * 8) {
        HIDE_SPRITES;
        printf("BOUNDS FAULT - %hu %hu", x, y);
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

static inline void _move_chain_down(sand_zone* this, uint8_t x, sand_chain* chain) {
    chain->y -= 1;
    _set_sand_color(this, x, chain->y, chain->value);
    _set_sand_color(this, x, chain->y + chain->length, DMG_WHITE);
}

static inline void _set_sand_color_column(sand_zone* this, uint8_t x, uint8_t y, uint8_t height, uint8_t value) {
    for (uint8_t i = 0; i < height; i++) {
        _set_sand_color(this, x, y + i, value);
    }
}

static inline sand_chain* _get_or_create_destination_chain(sand_zone* this, uint8_t x){
    sand_chain *chain = &this->sand_chains[x];
    if (chain->y > 0) {
        sand_chain* root = sand_chain__copy(chain);
        chain->next = root;
        chain->y = 0;
        chain->length = 0;
        chain->value = 0;
        return chain;
    }
    sand_chain *last_connected = sand_chain__get_last_connected(chain);
    return last_connected;
}

static inline void _slide_sand_chain(sand_zone* this, uint8_t x, uint8_t new_x) {
    sand_chain* current = this->sand_chains[x].next;
    if (current == NULL) return;
    if (current->y > 0) return; // sand-chain is still falling

    sand_chain* dest = _get_or_create_destination_chain(this, new_x);

    uint8_t target_y = dest->y + dest->length;
    uint8_t target_gap = sand_chain__get_gap_above(dest);

    uint8_t connected_length = sand_chain__get_connected_length(current);


    if (current->y + connected_length - 1 > target_y) {
        uint8_t amount_to_move = ((current->y + connected_length) - target_y) - 1;
        if (amount_to_move > target_gap) amount_to_move = target_gap;

        sand_chain* to_move = sand_chain__excise_chain(current, target_y + 1, amount_to_move);

        _set_sand_color_column(this, x, to_move->y, amount_to_move, 0);
        for (sand_chain* c = to_move; c != NULL; c = c->next) {
            c->y -= 1;
            _set_sand_color_column(this, new_x, c->y, c->length, c->value);
        }

        sand_chain* dest_next = dest->next;
        dest->next = to_move;
        sand_chain* to_move_top = sand_chain__get_last_connected(to_move);
        to_move_top->next = dest_next;

        sand_chain__try_to_combine(dest);
        sand_chain__try_to_combine(to_move_top);

        this->was_updated[x] = true;
        this->was_updated[new_x] = true;
    }
}

static inline void _collapse_empty_and_similar_chains(sand_zone* this) {
    uint8_t width = this->width * 8;
    for (uint8_t x = 0; x < width; x++) {
        sand_chain *current = &this->sand_chains[x];
        current = current->next;

        while (current != NULL && current->next != NULL) {
            sand_chain *next = current->next;
            if (current->length == 0) {
                current->y = next->y;
                current->length = next->length;
                current->value = next->value;
                current->next = next->next;
                next->next = NULL;
                sand_chain__delete(next);
                continue;
            }
            sand_chain__try_to_combine(current);
            current = current->next;
        }
    }
}

void sand_zone__update_sand(sand_zone* this) {
    _collapse_empty_and_similar_chains(this);

    uint8_t width = this->width * 8;

    bool* swap = this->needs_update;
    this->needs_update = this->was_updated;
    this->was_updated = swap;

    for (uint8_t x = 0; x < width; x++) {
        this->was_updated[x] = false;

        sand_chain *chain = &this->sand_chains[x];
        if (chain->y > 0) {
            _move_chain_down(this, x, chain);
            this->was_updated[x] = true;
        }

        sand_chain *prev_chain = chain;
        chain = chain->next;
        while (chain) {
            if (chain->y > prev_chain->y + prev_chain->length) {
                _move_chain_down(this, x, chain);
                this->was_updated[x] = true;
            }
            prev_chain = chain;
            chain = chain->next;
        }
    }

    for (uint8_t x = 0; x < width - 1; x++) {
        if (!this->needs_update[x] && !this->needs_update[x + 1]) continue;
        _slide_sand_chain(this, x + 1, x);
    }

    for (uint8_t x = width - 1; x > 0; x--) {
        if (!this->needs_update[x] && !this->needs_update[x - 1]) continue;
        _slide_sand_chain(this, x - 1, x);
    }

    _save_and_clear_tile_colors_cache();
}

void sand_zone__add_sand(sand_zone* this, uint8_t x, uint8_t y, uint8_t length, uint8_t value) {
    sand_chain* chain = &this->sand_chains[x];
    sand_chain* new_chain = sand_chain__add_chain(chain, y, length, value);

    uint8_t y_max = new_chain->y + new_chain->length;
    for (uint8_t y_at = new_chain->y; y_at < y_max; y_at++){
        _set_sand_color(this, x, y_at, value);
    }
    _save_and_clear_tile_colors_cache();
    this->was_updated[x] = true;
}

bool sand_zone__has_sand_at(sand_zone* this, uint8_t x, uint8_t y) {
    return _get_sand_color(this, x, y) > 0;
}

/******* PRIVATE CLASS *******/

void __initialize_inner_tiles(sand_zone* this) {
    uint8_t nb_tiles = this->width * this->height;
    this->inner_tiles = tile_set__alloc(nb_tiles);

    const uint8_t zero[] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    for (uint16_t z = 0; z < nb_tiles; z++) {
        set_bkg_data(this->inner_tiles->start + z, 1, zero);
    }


    for (uint8_t x = 0; x < this->width; x++) {
        for (uint8_t y = 0; y < this->height; y++) {
            uint8_t tile_offset = y * this->width + x;
            set_bkg_tile_xy(this->x + x, this->y + y, this->inner_tiles->start + tile_offset);
        }
    }
}

/******* PUBLIC CLASS *******/

sand_zone* sand_zone__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    sand_zone* tz = allocate(sizeof(sand_zone));
    tz->x = x;
    tz->y = y;
    tz->width = width;
    tz->height = height;

    __initialize_inner_tiles(tz);

    tz->sand_chains = sand_chain__new_array(tz->width * 8);

    tz->needs_update = allocate_array(tz->width * 8, sizeof(bool));
    tz->was_updated = allocate_array(tz->width * 8, sizeof(bool));

    return tz;
}

void sand_zone__delete(sand_zone* this) {
    tile_set__free(this->inner_tiles);
    free(this);
}
