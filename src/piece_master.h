#pragma once

#include "sand_zone.h"
#include <stdint.h>
#include <stdbool.h>

enum piece_type {
    O = 0, I, T, S, Z, L, J
};

struct piece_master {
    enum piece_type current_piece;
    uint8_t x;
    uint8_t y;
    uint8_t rotated;
    uint8_t color;
    struct sand_zone* zone;
    bool needs_new_piece;
    uint8_t previous_input;
    uint8_t current_input;
};

struct piece_master* new_piece_master(struct sand_zone* zone);
void piece_master__update(struct piece_master* this);
