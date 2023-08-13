#pragma once

#include "text_area.h"

typedef struct ui_game_over_struct {
    // private:
    text_area* _text;
} ui_game_over;

/******* INSTANCE *******/
void    ui_game_over__show(ui_game_over* this);

/******* CLASS *******/
ui_game_over*   ui_game_over__new(uint8_t x, uint8_t y, uint8_t width);
void            ui_game_over__delete(ui_game_over* this);