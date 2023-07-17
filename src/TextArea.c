#include "TextArea.h"
#include "allocate.h"
#include "../res/TILES_digits.h"
#include <stdio.h>
#include <gb/gb.h>
#include <stdbool.h>

struct tile_set *_digits;

static inline void _put_digit(struct TextArea* this, uint8_t digit) {
    uint8_t tile = digit + _digits->start;
    uint8_t x_offset = this->_cursor % this->width;
    uint8_t y_offset = (this->_cursor / this->width) % this->height;
    set_bkg_tile_xy(this->x + x_offset, this->y + y_offset, tile);
    this->_cursor++;
}

static void print_number(struct TextArea* this, int16_t value) {
    if (value < 0) {
        printf("TextArea does not support negative numbers!");
        exit(-1);
    }

    for (int16_t d = 10000; d >= 1; d /= 10) {
        int16_t v = value / d;
        if (v == 0 && d != 1) {
            continue;
        }

        int16_t digit = v % 10;
        _put_digit(this, digit);
    }
}

static void ta_reset(struct TextArea* this) {
    this->_cursor = 0;
}

/******* CLASS *******/

static inline void static_new() {
    if (_digits == NULL) {
        _digits = alloc_tile_set(TILES_digitsLen);
        tile_set__set_data(_digits, TILES_digits);
    }
}

static struct TextArea* new(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    static_new();

    struct TextArea* ta = allocate(sizeof(TextArea));
    ta->x = x;
    ta->y = y;
    ta->width = width;
    ta->height = height;
    return ta;
}

static void delete(struct TextArea* this) {
    free(this);
}

const struct TextAreaClass TextArea = {
    .new=&new,
    .delete=&delete,

    .print_number=&print_number,
    .reset=&ta_reset
};