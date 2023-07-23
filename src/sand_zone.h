#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "sand_chain.h"
#include "tile_set.h"

typedef struct sand_zone_struct {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
    tile_set* inner_tiles;
    sand_chain* sand_chains;

    bool* needs_update;
    bool* was_updated;
} sand_zone;

/******* INSTANCE *******/
void sand_zone__update_sand(sand_zone* this);
void sand_zone__add_sand(sand_zone* this, uint8_t x, uint8_t y, uint8_t length, uint8_t value);
bool sand_zone__has_sand_at(sand_zone* this, uint8_t x, uint8_t y);

/******* CLASS *******/
sand_zone* sand_zone__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void sand_zone__delete(sand_zone* this);