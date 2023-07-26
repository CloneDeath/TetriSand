#pragma bank 255

#include "sand_zone.h"
#include "../allocate.h"
#include "../tile_set.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include <gb/gb.h>

static inline void _move_chain_down(sand_zone* this, uint8_t x, sand_chain* chain) {
    chain->y -= 1;
    bitmap_area__set_color(this->bitmap_area, x, chain->y, chain->value);
    bitmap_area__set_color(this->bitmap_area, x, chain->y + chain->length, DMG_WHITE);
}

static inline void _set_sand_color_column(sand_zone* this, uint8_t x, uint8_t y, uint8_t height, uint8_t value) {
    for (uint8_t i = 0; i < height; i++) {
        bitmap_area__set_color(this->bitmap_area, x, y + i, value);
    }
}

static inline sand_chain* _get_or_create_destination_chain(sand_zone* this, uint8_t x){
    sand_chain *chain = &this->sand_chains[x];
    if (chain->y > 0) {
        sand_chain* root = sand_chain__copy(chain);
        chain->next = root;
        chain->y = 0;
        chain->length = 0;
        chain->value = 0;
        return chain;
    }
    sand_chain *last_connected = sand_chain__get_last_connected(chain);
    return last_connected;
}

static inline void _slide_sand_chain(sand_zone* this, uint8_t x, uint8_t new_x) {
    sand_chain* current = this->sand_chains[x].next;
    if (current == NULL) return;
    if (current->y > 0) return; // sand-chain is still falling

    sand_chain* dest = _get_or_create_destination_chain(this, new_x);

    uint8_t target_y = dest->y + dest->length;
    uint8_t target_gap = sand_chain__get_gap_above(dest);

    uint8_t connected_length = sand_chain__get_connected_length(current);


    if (current->y + connected_length - 1 > target_y) {
        uint8_t amount_to_move = ((current->y + connected_length) - target_y) - 1;
        if (amount_to_move > target_gap) amount_to_move = target_gap;

        sand_chain* to_move = sand_chain__excise_chain(current, target_y + 1, amount_to_move);

        _set_sand_color_column(this, x, to_move->y, amount_to_move, 0);
        for (sand_chain* c = to_move; c != NULL; c = c->next) {
            c->y -= 1;
            _set_sand_color_column(this, new_x, c->y, c->length, c->value);
        }

        sand_chain* dest_next = dest->next;
        dest->next = to_move;
        sand_chain* to_move_top = sand_chain__get_last_connected(to_move);
        to_move_top->next = dest_next;

        sand_chain__try_to_combine(dest);
        sand_chain__try_to_combine(to_move_top);

        this->was_updated[x] = true;
        this->was_updated[new_x] = true;
    }
}

static inline void _collapse_empty_and_similar_chains(sand_zone* this) {
    uint8_t width = this->width * 8;
    for (uint8_t x = 0; x < width; x++) {
        sand_chain *current = &this->sand_chains[x];
        current = current->next;

        while (current != NULL && current->next != NULL) {
            sand_chain *next = current->next;
            if (current->length == 0) {
                current->y = next->y;
                current->length = next->length;
                current->value = next->value;
                current->next = next->next;
                next->next = NULL;
                sand_chain__delete(next);
                continue;
            }
            sand_chain__try_to_combine(current);
            current = current->next;
        }
    }
}

static inline sand_chain* _get_matching_sand_chain(sand_zone* this, sand_chain* target, uint8_t x) {
    if (target->length == 0 || target->value) return NULL;
    uint8_t start = target->y;
    uint8_t end = target->y + target->length - 1;
    uint8_t value = target->value;

    sand_chain* current = this->sand_chains[x].next;
    while (current != NULL) {
        if (current->y > end + 1) return NULL;
        if (current->y + current->length < start - 1) {
            current = current->next;
            continue;
        }
        if (current->value != value) {
            current = current->next;
            continue;
        }
        return current;
    }
    return NULL;
}

static inline void _check_for_tetris(sand_zone* this) {
    int8_t width = this->width * 8;
    sand_chain** stack = allocate_array(width, sizeof(sand_chain*));
    stack[0] = this->sand_chains[0].next;
    int8_t stack_index = 0;

    while (stack_index >= 0 && stack_index < width) {
        sand_chain* current = stack[stack_index];
        sand_chain* next = _get_matching_sand_chain(this, current, stack_index+1);
        if (next == NULL) {
            current = current->next;
            stack[stack_index] = current;
            if (current == NULL) {
                stack_index--;
            }
            continue;
        }

        stack[stack_index+1] = next;
        stack_index++;
    }

    if (stack_index < 0) {
        free(stack);
        return;
    }

    for (int8_t x = 0; x < width; x++) {
        sand_chain* current = stack[x];
        _set_sand_color_column(this, x, current->y, current->length, DMG_WHITE);
        current->value = DMG_WHITE;
        current->length = 0;
    }

    free(stack);
}

/******* PUBLIC INSTANCE *******/

void sand_zone__update_sand(sand_zone* this) BANKED {
    _collapse_empty_and_similar_chains(this);

    uint8_t width = this->width * 8;

    bool* swap = this->needs_update;
    this->needs_update = this->was_updated;
    this->was_updated = swap;

    for (uint8_t x = 0; x < width; x++) {
        this->was_updated[x] = false;

        sand_chain *chain = &this->sand_chains[x];
        if (chain->y > 0) {
            _move_chain_down(this, x, chain);
            this->was_updated[x] = true;
        }

        sand_chain *prev_chain = chain;
        chain = chain->next;
        while (chain) {
            if (chain->y > prev_chain->y + prev_chain->length) {
                _move_chain_down(this, x, chain);
                this->was_updated[x] = true;
            }
            prev_chain = chain;
            chain = chain->next;
        }
    }

    for (uint8_t x = 0; x < width - 1; x++) {
        if (!this->needs_update[x] && !this->needs_update[x + 1]) continue;
        _slide_sand_chain(this, x + 1, x);
    }

    for (uint8_t x = width - 1; x > 0; x--) {
        if (!this->needs_update[x] && !this->needs_update[x - 1]) continue;
        _slide_sand_chain(this, x - 1, x);
    }

    _check_for_tetris(this);
    bitmap_area__flush_cache();
}

void sand_zone__add_sand(sand_zone* this, uint8_t x, uint8_t y, uint8_t length, uint8_t value) BANKED {
    sand_chain* chain = &this->sand_chains[x];
    sand_chain* new_chain = sand_chain__add_chain(chain, y, length, value);

    uint8_t y_max = new_chain->y + new_chain->length;
    for (uint8_t y_at = new_chain->y; y_at < y_max; y_at++){
        bitmap_area__set_color(this->bitmap_area, x, y_at, value);
    }
    bitmap_area__flush_cache();
    this->was_updated[x] = true;
}

bool sand_zone__has_sand_at(sand_zone* this, uint8_t x, uint8_t y) BANKED {
    return bitmap_area__get_color(this->bitmap_area, x, y) > 0;
}

/******* PUBLIC CLASS *******/

sand_zone* sand_zone__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height) BANKED {
    sand_zone* tz = allocate(sizeof(sand_zone));
    tz->x = x;
    tz->y = y;
    tz->width = width;
    tz->height = height;

    tz->bitmap_area = bitmap_area__new(x, y, width, height);

    tz->sand_chains = sand_chain__new_array(tz->width * 8);

    tz->needs_update = allocate_array(tz->width * 8, sizeof(bool));
    tz->was_updated = allocate_array(tz->width * 8, sizeof(bool));

    return tz;
}

void sand_zone__delete(sand_zone* this) BANKED {
    bitmap_area__delete(this->bitmap_area);
    for (uint8_t x = 0; x < this->width * 8; x++) {
        sand_chain__delete(this->sand_chains + x);
    }
    free(this->needs_update);
    free(this->was_updated);
    free(this);
}
