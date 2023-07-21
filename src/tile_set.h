#pragma once

#include "stdint.h"

typedef struct tile_set_struct {
    uint8_t start;
    uint8_t count;
} tile_set;

/******* PUBLIC *******/

void tile_set__set_data(tile_set* this, const uint8_t *data);

/******* CLASS *******/

tile_set* tile_set__alloc(uint8_t nb_tiles);
void tile_set__free(tile_set* this);
