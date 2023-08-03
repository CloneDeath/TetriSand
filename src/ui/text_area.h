#pragma once

#include "../engine/tile_set.h"

typedef struct text_area_struct {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    // private:
    uint8_t _cursor;
} text_area;

/******* INSTANCE *******/

void text_area__pop_char(text_area* this);
void text_area__push_digit(text_area* this, uint8_t digit);
void text_area__print_number(text_area* this, int16_t value);
void text_area__reset(text_area* this);

/******* CLASS *******/

text_area* text_area__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void text_area__delete(text_area* this);