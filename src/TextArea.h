#pragma once

#include "tile_set.h"

struct TextArea {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    void (*print_number)(struct TextArea * this, int16_t

    digits);

    void reset(struct TextArea * this);

    // private:
    uint8_t _cursor;

    void _put_digit(struct TextArea * this, uint8_t

    digit);
};

extern struct TextAreaClass {
    struct TextArea* (*new)(uint8_t x, uint8_t
    y, uint8_t width, uint8_t
    height);
    void (*delete)(struct TextArea* this);

    // private:
    struct tile_set *_digits;
} TextArea;