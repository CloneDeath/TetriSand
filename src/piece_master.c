#include "piece_master.h"
#include <gb/gb.h>
#include <stdlib.h>
#include <rand.h>
#include "../res/tetromino.h"

const uint8_t NUMBER_OF_PIECES = 7;

struct piece_master* new_piece_master(struct sand_zone* zone) {
    set_sprite_data(0, 4, Tetromino);

    struct piece_master* this = malloc(sizeof (struct piece_master));
    this->x = 0;
    this->y = 0;
    this->current_piece = O;
    this->zone = zone;
    this->needs_new_piece = true;
    return this;
}

static inline uint8_t _get_center_x(struct piece_master* this) {
    return (this->zone->width * 8) / 2;
}

static inline uint8_t _get_top_y(struct piece_master* this) {
    return this->zone->height * 8;
}

static inline void _set_color(struct piece_master* this, uint8_t color) {
    this->color = color;
    for (uint8_t i = 0; i < 4; i++) {
        set_sprite_tile(i, color);
    }
}

static inline void _move_sprite_to(struct piece_master* this, uint8_t sn, uint8_t x, uint8_t y) {
    uint8_t screen_x = ((this->zone->x + 2) * 8) + x - 4;
    uint8_t screen_y = ((this->zone->y + 2 + this->zone->height) * 8) - y + 4;
    move_sprite(sn, screen_x, screen_y);
}

static inline void _adjust_sprites(struct piece_master* this) {
    switch (this->current_piece) {
        case O:
        default:
            _move_sprite_to(this, 0, this->x - 8, this->y - 8);
            _move_sprite_to(this, 1, this->x + 0, this->y - 8);
            _move_sprite_to(this, 2, this->x - 8, this->y + 0);
            _move_sprite_to(this, 3, this->x + 0, this->y + 0);
            return;
        case I:
            if (this->rotated % 2 == 0) {
                _move_sprite_to(this, 0, this->x - 16, this->y);
                _move_sprite_to(this, 1, this->x - 8, this->y);
                _move_sprite_to(this, 2, this->x + 0, this->y);
                _move_sprite_to(this, 3, this->x + 8, this->y);
            } else {
                _move_sprite_to(this, 0, this->x, this->y - 16);
                _move_sprite_to(this, 1, this->x, this->y - 8);
                _move_sprite_to(this, 2, this->x, this->y + 0);
                _move_sprite_to(this, 3, this->x, this->y + 8);
            }
            return;
    }
}

static inline void _add_square(struct piece_master* this, uint8_t x, uint8_t y){
    for (uint8_t i = 0; i < 8; i++){
        sand_zone__add_sand(this->zone, x + i - 4, y - 4, 8, this->color);
    }
}

void piece_master__update(struct piece_master* this) {
    if (this->needs_new_piece) {
        this->needs_new_piece = false;
        this->x = _get_center_x(this);
        this->y = _get_top_y(this);
        this->current_piece = rand() % NUMBER_OF_PIECES;
        this->rotated = rand() % 4;
        _set_color(this, (rand() % 3) + 1);
    }

    this->previous_input = this->current_input;
    this->current_input = joypad();

    if (this->current_input & J_LEFT) {
        this->x -= 1;
    }
    if (this->current_input & J_RIGHT) {
        this->x += 1;
    }
    if (this->x < 8) this->x = 8;
    uint8_t max_width = this->zone->width * 8 - 8;
    if (this->x > max_width) this->x = max_width;
    if (this->current_input & J_A && !(this->previous_input & J_A)) {
        this->rotated = (this->rotated - 1) % 4;
    }
    if (this->current_input & J_B && !(this->previous_input & J_B)) {
        this->rotated = (this->rotated + 1) % 4;
    }

    this->y -= 1;
    _adjust_sprites(this);

    if (this->y <= 8) {
        _add_square(this, this->x - 4, this->y - 4);
        _add_square(this, this->x + 4, this->y - 4);
        _add_square(this, this->x - 4, this->y + 4);
        _add_square(this, this->x + 4, this->y + 4);
        this->needs_new_piece = true;
    }
}