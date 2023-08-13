#pragma once

#include "tile_set.h"

#include <gb/gb.h>

typedef struct bitmap_area_struct {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
    tile_set* inner_tiles;
} bitmap_area;

/******* PUBLIC INSTANCE *******/
uint8_t bitmap_area__get_color(bitmap_area* this, uint8_t x, uint8_t y) BANKED;
void bitmap_area__set_color(bitmap_area* this, uint8_t x, uint8_t y, uint8_t value) BANKED;

/******* PUBLIC CLASS *******/
bitmap_area* bitmap_area__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height) BANKED;
void bitmap_area__delete(bitmap_area* this) BANKED;

void bitmap_area__flush_cache() BANKED;