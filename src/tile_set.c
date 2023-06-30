#include "tile_set.h"
#include "stdlib.h"
#include "stdbool.h"

bool tile_is_available[127];

bool is_range_available(uint8_t start, uint8_t count) {
    for (uint8_t i = start; i < start + count; i++) {
        bool available = tile_is_available[i];
        if (!available) return false;
    }
    return true;
}

void reserve_range(uint8_t start, uint8_t count) {
    for (uint8_t i = start; i < start + count; i++) {
        tile_is_available[i] = false;
    }
}

void reserve_tiles(struct tile_set* tiles, uint8_t nb_tiles) {
    for (uint8_t i = 0; i < 127; i++){
        bool available = is_range_available(i, nb_tiles);
        if (!available) continue;

        reserve_range(i, nb_tiles);
        tiles->start = i;
        tiles->count = nb_tiles;
        return;
    }
}

struct tile_set* alloc_tile_set(uint8_t nb_tiles) {
    struct tile_set* tiles = malloc(sizeof(struct tile_set));
    reserve_tiles(tiles, nb_tiles);
    return tiles;
}

void free_range(uint8_t start, uint8_t count) {
    for (uint8_t i = start; i < start + count; i++) {
        tile_is_available[i] = true;
    }
}

void free_tile_set(struct tile_set* this) {
    free_range(this->start, this->count);
    free(this);
}