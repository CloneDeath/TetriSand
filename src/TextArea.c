#include "TextArea.h"
#include "allocate.h"
#include "../res/TILES_digits.h"
#include <stdio.h>
#include <gb/gb.h>

static void print_number(struct TextArea* this, int16_t value) {
    if (value < 0) {
        printf("TextArea does not support negative numbers!");
        exit(-1);
    }
    if (value < 10) {
        this->_put_digit(this, value);
    }
}

static void ta_reset(struct TextArea* this) {
    this->_cursor = 0;
}

static void _put_digit(struct TextArea* this, uint8_t digit) {
    uint8_t tile = digit + TextArea._digits->start;
    uint8_t x_offset = this->_cursor % this->width;
    uint8_t y_offset = (this->_cursor / this->width) % this->height;
    set_bkg_tile_xy(this->x + x_offset, this->y + y_offset, tile);
    this->_cursor++;
}

static inline void static_new() {
    if (TextArea._digits == NULL) {
        TextArea._digits = alloc_tile_set(TILES_digitsLen);
        tile_set__set_data(TextArea._digits, TILES_digits);
    }
}

static struct TextArea* new(uint8_t x, uint8_t y, uint8_t width, uint8_t height){
    static_new();

    struct TextArea* ta = allocate(sizeof(TextArea));
    ta->print_number = &print_number;
    ta->reset = &ta_reset;
    ta->_put_digit = &_put_digit;

    ta->x = x;
    ta->y = y;
    ta->width = width;
    ta->height = height;
    return ta;
}

static void delete(struct TextArea* this) {
    free(this);
}

struct TextAreaClass TextArea = {
    .new=&new,
    .delete=&delete,
    ._digits=NULL
};