#include "ui_lines.h"

#include "../engine/allocate.h"

#define SAND_PER_LINE 640

/******* INSTANCE *******/
void ui_lines__add_sand(ui_lines* this, uint8_t amount) {
    this->_sand += amount;
    if (this->_sand >= SAND_PER_LINE) {
        this->_sand -= SAND_PER_LINE;
        this->_lines += 1;

        text_area__reset(this->_lines_value);
        text_area__print_number(this->_lines_value, this->_lines);
    }
}

/******* CLASS *******/

ui_lines* ui_lines__new(uint8_t x, uint8_t y, uint8_t width) {
    ui_lines* this = allocate(sizeof(ui_lines));
    this->_sand = 0;
    this->_lines = 0;
    this->_lines_label = text_area__new(x, y, width, 1);
    this->_lines_value = text_area__new(x, y+1, width, 1);
}

void ui_lines__delete(ui_lines* this) {
    text_area__delete(this->_lines_value);
    text_area__delete(this->_lines_label);
    free(this);
}