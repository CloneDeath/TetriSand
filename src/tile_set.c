#include "tile_set.h"
#include <stdlib.h>
#include <stdbool.h>
#include <gb/gb.h>
#include "allocate.h"

/******* PRIVATE CLASS MEMBERS *******/

#define MAX_TILES 255
bool tile_in_use[MAX_TILES];

/******* PRIVATE CLASS FUNCTIONS *******/

static inline bool __is_range_available(uint8_t start, uint8_t count) {
    for (uint8_t i = start; i < start + count; i++) {
        bool in_use = tile_in_use[i];
        if (in_use) return false;
    }
    return true;
}

static inline void __reserve_range(uint8_t start, uint8_t count) {
    for (uint8_t i = start; i < start + count; i++) {
        tile_in_use[i] = true;
    }
}

static inline void __reserve_tiles(tile_set* tiles, uint8_t nb_tiles) {
    for (uint8_t i = 0; i < MAX_TILES; i++){
        bool available = __is_range_available(i, nb_tiles);
        if (!available) continue;

        __reserve_range(i, nb_tiles);
        tiles->start = i;
        tiles->count = nb_tiles;
        return;
    }
}

static inline void __free_range(uint8_t start, uint8_t count) {
    for (uint8_t i = start; i < start + count; i++) {
        tile_in_use[i] = false;
    }
}

/******* PUBLIC *******/

void tile_set__set_data(tile_set* this, const uint8_t *data) {
    set_bkg_data(this->start, this->count, data);
}

/******* CLASS *******/

tile_set* tile_set__alloc(uint8_t nb_tiles) {
    tile_set* tiles = allocate(sizeof(tile_set));
    __reserve_tiles(tiles, nb_tiles);
    return tiles;
}

void tile_set__free(tile_set* this) {
    __free_range(this->start, this->count);
    free(this);
}
