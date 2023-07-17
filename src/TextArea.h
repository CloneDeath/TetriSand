#pragma once

#include "tile_set.h"

struct TextArea {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    // private:
    uint8_t _cursor;
};

void TextArea__clear(struct TextArea* this);

extern const struct TextAreaClass {
    struct TextArea* (*new)(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void (*delete)(struct TextArea* this);

    void (*print_number)(struct TextArea *this, int16_t value);
    void (*reset)(struct TextArea *this);
} TextArea;