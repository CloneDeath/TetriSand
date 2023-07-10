#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "sand_chain.h"

struct sand_zone {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
    struct tile_set* inner_tiles;
    struct sand_chain* sand_chains;

    bool* needs_update;
    bool* was_updated;
};

struct sand_zone* new_sand_zone(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void delete_sand_zone(struct sand_zone* this);

void sand_zone__update_sand(struct sand_zone* this);
void sand_zone__add_sand(struct sand_zone* this, uint8_t x, uint8_t y, uint8_t length, uint8_t value);
