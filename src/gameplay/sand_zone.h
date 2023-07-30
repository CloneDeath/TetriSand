#pragma once

#include "sand_chain.h"
#include "sand_chain_list.h"

#include <stdbool.h>
#include <stdint.h>
#include "../tile_set.h"
#include "../engine/bitmap_area.h"

typedef struct sand_zone_struct {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
    sand_chain* sand_chains;
    bitmap_area* bitmap_area;

    bool* needs_update;
    bool* was_updated;
} sand_zone;

/******* INSTANCE *******/
sand_chain_list*    sand_zone__get_connected_chains_in_column(sand_zone* this, sand_chain* chain, uint8_t x) BANKED;
void                sand_zone__update_sand(sand_zone* this) BANKED;
void                sand_zone__add_sand(sand_zone* this, uint8_t x, uint8_t y, uint8_t length, uint8_t value) BANKED;
bool                sand_zone__has_sand_at(sand_zone* this, uint8_t x, uint8_t y) BANKED;

/******* CLASS *******/
sand_zone* sand_zone__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height) BANKED;
void sand_zone__delete(sand_zone* this) BANKED;