#pragma bank 255

#include "piece_master.h"
#include "allocate.h"

#include "../res/tetromino.h"

#include <stdlib.h>
#include <rand.h>

#include <gb/gb.h>

const uint8_t NUMBER_OF_PIECES = 7;

/**** OFFSET ****/
typedef struct offset_struct {
    int8_t x;
    int8_t y;
} offset;

const offset offsets[7][4] = {
    {{0,  0},  {0,  1}, {1, 0},  {1,  1}}, // O
    {{-2, 0},  {-1, 0}, {0, 0},  {1,  0}}, // I
    {{-1, 0},  {0,  0}, {1, 0},  {0,  1}}, // T
    {{-1, -1}, {0,  0}, {0, -1}, {1,  0}}, // S
    {{-1, 0},  {0,  0}, {0, -1}, {1,  -1}}, // Z
    {{0,  1},  {0,  0}, {0, -1}, {1,  -1}}, // L
    {{0,  1},  {0,  0}, {0, -1}, {-1, -1}}, // J
};

static inline int8_t _get_sub_piece_x(piece_master* this, uint8_t sn) {
    const offset *current = &(offsets[this->current_piece][sn]);
    uint8_t rot = this->rotated % 4;
    if (rot == 0) return current->x * 8;
    if (rot == 1) return -current->y * 8;
    if (rot == 2) return -current->x * 8;
    return current->y * 8;
}

static inline uint8_t _get_sub_piece_y(piece_master* this, uint8_t sn) {
    const offset *current = &(offsets[this->current_piece][sn]);
    uint8_t rot = this->rotated % 4;
    if (rot == 0) return current->y * 8;
    if (rot == 1) return current->x * 8;
    if (rot == 2) return -current->y * 8;
    return -current->x * 8;
}

/******* PRIVATE INSTANCE *******/

static inline uint8_t _get_center_x(piece_master* this) {
    return (this->zone->width * 8) / 2;
}

static inline uint8_t _get_top_y(piece_master* this) {
    return this->zone->height * 8;
}

static inline void _set_color(piece_master *this, uint8_t color) {
    this->color = color;
    for (uint8_t i = 0; i < 4; i++) {
        set_sprite_tile(i, color);
    }
}

static inline void _move_sprite_to(piece_master *this, uint8_t sn, uint8_t x, uint8_t y) {
    uint8_t screen_x = ((this->zone->x + 1) * 8) + x;
    uint8_t screen_y = ((this->zone->y + 1 + this->zone->height) * 8) - y;
    move_sprite(sn, screen_x, screen_y);
}


static inline uint8_t _get_min_x(piece_master* this) {
    int8_t min_x = 0;
    for (uint8_t i = 0; i < 4; i++) {
        int8_t x = _get_sub_piece_x(this, i);
        if (x < min_x) {
            min_x = x;
        }
    }
    return -1 * min_x;
}

static inline uint8_t _get_min_y(piece_master* this) {
    int8_t min_y = 0;
    for (uint8_t i = 0; i < 4; i++) {
        int8_t y = _get_sub_piece_y(this, i);
        if (y < min_y) {
            min_y = y;
        }
    }
    return -1 * min_y;
}

static inline uint8_t _get_max_x(piece_master* this) {
    int8_t max_x = 0;
    for (uint8_t i = 0; i < 4; i++) {
        int8_t x = _get_sub_piece_x(this, i);
        if (x > max_x) {
            max_x = x;
        }
    }
    return (this->zone->width * 8) - (max_x + 8);
}

static inline uint8_t _get_max_y(piece_master* this) {
    int8_t max_y = 0;
    for (uint8_t i = 0; i < 4; i++) {
        int8_t y = _get_sub_piece_y(this, i);
        if (y > max_y) {
            max_y = y;
        }
    }
    return (this->zone->height * 8) - (max_y + 8);
}

static inline void _adjust_sprites(piece_master *this) {
    for (uint8_t i = 0; i < 4; i++) {
        _move_sprite_to(this, i, this->x + _get_sub_piece_x(this, i), this->y + _get_sub_piece_y(this, i));
    }
}

static inline void _add_square(piece_master *this, uint8_t x, uint8_t y) {
    for (uint8_t i = 0; i < 8; i++) {
        sand_zone__add_sand(this->zone, x + i, y, 8, this->color);
    }
}

static inline void _spawn_sand(piece_master *this) {
    for (uint8_t i = 0; i < 4; i++) {
        int8_t x = _get_sub_piece_x(this, i);
        int8_t y = _get_sub_piece_y(this, i);

        _add_square(this, this->x + x, this->y + y);
    }
}

static inline bool _sub_piece_is_touching_sand(piece_master* this, uint8_t sb) {
    uint8_t piece_x = this->x + _get_sub_piece_x(this, sb);
    uint8_t piece_y = this->y + _get_sub_piece_y(this, sb);
    for (uint8_t x = piece_x; x < piece_x + 8; x++) {
        if (sand_zone__has_sand_at(this->zone, x, piece_y - 1)) {
            return true;
        }
    }
    return false;
}

static inline bool _piece_is_touching_sand(piece_master* this) {
    for (uint8_t i = 0; i < 4; i++) {
        if (_sub_piece_is_touching_sand(this, i)) {
            return true;
        }
    }
    return false;
}

/******* PUBLIC INSTANCE *******/

void piece_master__update(piece_master *this) BANKED {
    if (this->game_over) {
        _adjust_sprites(this);
        return;
    }

    if (this->needs_new_piece) {
        this->needs_new_piece = false;
        this->x = _get_center_x(this);
        this->y = _get_top_y(this);
        this->current_piece = rand() % NUMBER_OF_PIECES;
        this->rotated = rand() % 4;
        _set_color(this, (rand() % 3) + 1);
        if (_piece_is_touching_sand(this)) {
            this->game_over = true;
            return;
        }
    }

    this->previous_input = this->current_input;
    this->current_input = joypad();

    if (this->current_input & J_LEFT && this->x > _get_min_x(this)) {
        this->x -= 1;
        if (_piece_is_touching_sand(this)) {
            this->x += 1;
        }
    }
    if (this->current_input & J_RIGHT && this->x < _get_max_x(this)) {
        this->x += 1;
        if (_piece_is_touching_sand(this)) {
            this->x -= 1;
        }
    }
    if (this->current_input & J_A && !(this->previous_input & J_A)) {
        this->rotated = (this->rotated - 1) % 4;
    }
    if (this->current_input & J_B && !(this->previous_input & J_B)) {
        this->rotated = (this->rotated + 1) % 4;
    }
    if (this->x < _get_min_x(this)) this->x = _get_min_x(this);
    if (this->x > _get_max_x(this)) this->x = _get_max_x(this);

    this->y -= 1;
    _adjust_sprites(this);

    if (this->y <= _get_min_y(this) || _piece_is_touching_sand(this)) {
        if (this->y > _get_max_y(this)) {
            this->game_over = true;
            return;
        }
        _spawn_sand(this);
        this->needs_new_piece = true;
    }
}

/******* CLASS *******/

piece_master *piece_master__new(sand_zone *zone) BANKED {
    set_sprite_data(0, 4, Tetromino);

    piece_master* this = allocate(sizeof(piece_master));
    this->game_over = false;
    this->x = 0;
    this->y = 0;
    this->current_piece = O;
    this->zone = zone;
    this->needs_new_piece = true;
    return this;
}

void piece_master__delete(piece_master* this) BANKED {
    free(this);
}