#include "text_area.h"
#include "allocate.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <gb/gb.h>
#include "../res/TILES_digits.h"
#include "../res/TILES_blank.h"

/******* PRIVATE CLASS MEMBERS *******/

tile_set* _digits;
tile_set* _blank;

/******* PUBLIC INSTANCE *******/

void text_area__pop_char(text_area* this) {
    assert(this->_cursor > 0);

    this->_cursor--;

    uint8_t x_offset = this->_cursor % this->width;
    uint8_t y_offset = (this->_cursor / this->width) % this->height;
    set_bkg_tile_xy(this->x + x_offset, this->y + y_offset, _blank->start);
}

void text_area__put_digit(text_area* this, uint8_t digit) {
    assert(digit < 10);
    assert(digit >= 0);

    uint8_t tile = digit + _digits->start;
    uint8_t x_offset = this->_cursor % this->width;
    uint8_t y_offset = (this->_cursor / this->width) % this->height;
    set_bkg_tile_xy(this->x + x_offset, this->y + y_offset, tile);
    this->_cursor++;
}

void text_area__print_number(text_area* this, int16_t value) {
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
        __put_digit(this, digit);
    }
}

void text_area__reset(text_area* this) {
    while (this->_cursor > 0) {
        text_area__pop_char(this);
    }
}

/******* PRIVATE INSTANCE *******/

void text_area__clear(text_area* this) {
    this->_cursor = 0;
    for (uint8_t x = 0; x < this->width; x++) {
        for (uint8_t y = 0; y < this->height; y++) {
            uint8_t tile = _blank->start;
            set_bkg_tile_xy(this->x + x, this->y + y, tile);
        }
    }
}

/******* PUBLIC CLASS *******/

static inline void __class_new() {
    if (_digits == NULL) {
        _digits = tile_set__alloc(TILES_digitsLen);
        tile_set__set_data(_digits, TILES_digits);

        _blank = tile_set__alloc(TILES_blankLen);
        tile_set__set_data(_blank, TILES_blank);
    }
}

text_area* text_area__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    __class_new();

    text_area* ta = allocate(sizeof(text_area));
    ta->x = x;
    ta->y = y;
    ta->width = width;
    ta->height = height;

    text_area__clear(ta);
    return ta;
}

void text_area__delete(text_area* this) {
    free(this);
}