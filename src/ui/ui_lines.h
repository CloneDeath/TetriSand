#pragma once

#include <stdint.h>
#include "text_area.h"

typedef struct ui_lines_struct {
    // private:
    text_area* _lines_label;
    text_area* _lines_value;
    uint16_t _sand;
    uint16_t _lines;
} ui_lines;

/******* INSTANCE *******/
void        ui_lines__add_sand(ui_lines* this, uint8_t amount);

/******* CLASS *******/

ui_lines*   ui_lines__new(uint8_t x, uint8_t y, uint8_t width)
void        ui_lines__delete(ui_lines* this);