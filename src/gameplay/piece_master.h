#pragma once

#include "sand_zone.h"
#include <stdint.h>
#include <stdbool.h>

enum piece_type {
    O = 0, I, T, S, Z, L, J
};

typedef struct piece_master_struct {
    // public:
    bool game_over;

    // private:
    enum piece_type current_piece;
    uint8_t x;
    uint8_t y;
    uint8_t rotated;
    uint8_t color;
    sand_zone* zone;
    bool needs_new_piece;
    uint8_t previous_input;
    uint8_t current_input;
} piece_master;

/******* INSTANCE *******/

void piece_master__update(piece_master* this) BANKED;

/******* CLASS *******/

piece_master* piece_master__new(sand_zone* zone) BANKED;
void piece_master__delete(piece_master* this) BANKED;