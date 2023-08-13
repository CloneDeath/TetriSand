#include "ui_game_over.h"

#include "allocate.h"

/******* INSTANCE *******/
void ui_game_over__show(ui_game_over* this) {
    text_area__print_text(this->_text, "GAME\n  OVER");
}

/******* CLASS *******/
ui_game_over* ui_game_over__new(uint8_t x, uint8_t y, uint8_t width) {
    ui_game_over* this = allocate(sizeof(ui_game_over));
    this->_text = text_area__new(x, y, width, 2);
    return this;
}

void ui_game_over__delete(ui_game_over* this) {
    text_area__delete(this->_text);
    free(this);
}