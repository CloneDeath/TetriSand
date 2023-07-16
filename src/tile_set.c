#include "tile_set.h"
#include <stdlib.h>
#include <stdbool.h>
#include <gb/gb.h>
#include "allocate.h"

#define MAX_TILES 255
bool tile_in_use[MAX_TILES];

bool is_range_available(uint8_t start, uint8_t count) {
    for (uint8_t i = start; i < start + count; i++) {
        bool in_use = tile_in_use[i];
        if (in_use) return false;
    }
    return true;
}

void reserve_range(uint8_t start, uint8_t count) {
    for (uint8_t i = start; i < start + count; i++) {
        tile_in_use[i] = true;
    }
}

void reserve_tiles(struct tile_set* tiles, uint8_t nb_tiles) {
    for (uint8_t i = 0; i < MAX_TILES; i++){
        bool available = is_range_available(i, nb_tiles);
        if (!available) continue;

        reserve_range(i, nb_tiles);
        tiles->start = i;
        tiles->count = nb_tiles;
        return;
    }
}

struct tile_set* alloc_tile_set(uint8_t nb_tiles) {
    struct tile_set* tiles = allocate(sizeof(struct tile_set));
    reserve_tiles(tiles, nb_tiles);
    return tiles;
}

void free_range(uint8_t start, uint8_t count) {
    for (uint8_t i = start; i < start + count; i++) {
        tile_in_use[i] = false;
    }
}

void free_tile_set(struct tile_set* this) {
    free_range(this->start, this->count);
    free(this);
}

void tile_set__set_data(struct tile_set* this, const uint8_t *data) {
    set_bkg_data(this->start, this->count, data);
}