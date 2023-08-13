#pragma bank 255

#include "sand_zone.h"
#include "sand_chain_list.h"
#include "../engine/allocate.h"
#include "../engine/tile_set.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include <gb/gb.h>

static inline void _set_sand_color_column(sand_zone* this, uint8_t x, uint8_t y, uint8_t height, uint8_t value) {
    for (uint8_t i = 0; i < height; i++) {
        bitmap_area__set_color(this->bitmap_area, x, y + i, value);
    }
}

static inline void _delete_next_sand_chain(sand_zone* this, uint8_t x, sand_chain* previous) {
    sand_chain* next = previous->next;
    _set_sand_color_column(this, x, next->y, next->length, DMG_WHITE);
    next->length = 0;
    sand_chain__try_to_combine(previous);
}

static inline void _move_next_chain_down(sand_zone* this, uint8_t x, sand_chain* chain) {
    sand_chain* next = chain->next;
    if (next == NULL) return;
    if (next->y > chain->y + chain->length) {
        this->was_updated[x] = true;
        this->start_to_end_triggered = false;

        next->y -= 1;
        bitmap_area__set_color(this->bitmap_area, x, next->y, next->value);
        bitmap_area__set_color(this->bitmap_area, x, next->y + next->length, DMG_WHITE);

        sand_chain__try_to_combine(chain);
    }
}

static inline sand_chain* _get_or_create_destination_chain(sand_zone* this, uint8_t x){
    sand_chain *chain = &this->sand_chains[x];
    sand_chain *last_connected = sand_chain__get_last_connected(chain);
    return last_connected;
}

static inline void _slide_sand_chain(sand_zone* this, uint8_t x, uint8_t new_x) {
    sand_chain* current = &this->sand_chains[x];
    if (current->next == NULL) return;
    if (current->next->y > 0) return; // sand-chain is still falling

    sand_chain* dest = _get_or_create_destination_chain(this, new_x);

    uint8_t target_y = dest->y + dest->length;
    uint8_t target_gap = sand_chain__get_gap_above(dest);

    uint8_t connected_length = sand_chain__get_connected_length(current);

    if (connected_length - 1 > target_y) {
        uint8_t amount_to_move = (connected_length - target_y) - 1;
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
        this->start_to_end_triggered = false;
    }
}

static inline void _detect_empty_and_similar_chains(sand_zone* this) {
    uint8_t width = this->width * 8;
    for (uint8_t x = 0; x < width; x++) {
        sand_chain* current = this->sand_chains[x].next;
        while (current != NULL) {
            assert(current->length > 0);
            current = current->next;
        }
    }
}

static inline sand_chain_list* __get_same_color_chains_adjacent_to(sand_zone* this, sand_chain* chain, uint8_t x) {
    sand_chain_list* chains = sand_chain_list__new();
    if (x > 0) {
        sand_chain_list* left = sand_zone__get_connected_chains_in_column(this, chain, x - 1);
        chains = sand_chain_list__combine(chains, left);
    }

    if (x < this->width * 8 - 1) {
        sand_chain_list* right = sand_zone__get_connected_chains_in_column(this, chain, x + 1);
        chains = sand_chain_list__combine(chains, right);
    }

    return chains;
}

static inline void _clear_out_chains_in_family(sand_zone* this, uint8_t family) {
    uint8_t width = this->width * 8;

    for (uint8_t x = 0; x < width; x++) {
        sand_chain* previous = &this->sand_chains[x];
        sand_chain* current = previous->next;
        while (current != NULL) {
            if (current->family == family) {
                ui_lines__add_sand(this->_lines, current->length);
                _delete_next_sand_chain(this, x, previous);
            }
            previous = current;
            current = current->next;
        }
    }
}

static inline bool _family_reached_end(sand_zone* this, uint8_t family) {
    uint8_t width = this->width * 8;
    sand_chain* end = this->sand_chains[width-1].next;

    while (end != NULL) {
        if (end->family == family) return true;
        end = end->next;
    }
    return false;
}

uint16_t to_process_length = 0;
static inline void _check_for_start_to_end_path_for_chain(sand_zone* this, sand_chain* current, uint8_t family) {
    sand_chain_list* to_process = sand_chain_list__new();
    to_process_length = 0;

    sand_chain_list__push_front(to_process, current, 0);

    while (sand_chain_list__has_any(to_process)) {
        to_process_length = to_process->length;

        sand_chain_reference* ref = sand_chain_list__pop_front(to_process);
        if (ref->chain->family == family) {
            sand_chain_reference__delete_single(ref);
            continue;
        }
        ref->chain->family = family;

        sand_chain_list* adjacent = __get_same_color_chains_adjacent_to(this, ref->chain, ref->x);
        to_process = sand_chain_list__combine(to_process, adjacent);
    }

    to_process_length = 0;
    sand_chain_list__delete(to_process);

    if (_family_reached_end(this, family)) {
        _clear_out_chains_in_family(this, family);
    }
}


static inline void _clear_all_families(sand_zone* this) {
    uint8_t width = this->width * 8;
    for (uint8_t i = 0; i < width; i++) {
        sand_chain * current = this->sand_chains[i].next;
        while (current != NULL) {
            current->family = 0;
            current = current->next;
        }
    }
}
static inline void _check_for_start_to_end_path(sand_zone* this) {
    this->start_to_end_triggered = true;

    _clear_all_families(this);

    sand_chain* current = this->sand_chains[0].next;

    uint8_t current_family = 0;
    while (current != NULL) {
        if (++current_family >= 32) {
            current_family = 1;
            _clear_all_families(this);
        }
        _check_for_start_to_end_path_for_chain(this, current, current_family);
        current = current->next;
    }
}

static inline bool _should_check_for_start_to_end_path(sand_zone* this) {
    if (this->start_to_end_triggered) return false;
    uint8_t width = this->width * 8;
    for (uint8_t i = 0; i < width; i++) {
        if (this->was_updated[i]) return false;
    }
    return true;
}

/******* PUBLIC INSTANCE *******/

sand_chain_list* sand_zone__get_connected_chains_in_column(sand_zone* this, sand_chain* chain, uint8_t x) BANKED {
    uint8_t color = chain->value;
    sand_chain_list* chains = sand_chain_list__new();

    sand_chain* current = this->sand_chains[x].next;
    while (current != NULL) {
        if (current->value != color) {
            current = current->next;
            continue;
        }

        int8_t adjacency = sand_chain__get_adjacency(chain, current);
        if (adjacency > 0) break;
        if (adjacency < 0) {
            current = current->next;
            continue;
        }

        sand_chain_list__push_front(chains, current, x);
        current = current->next;
    }

    return chains;
}

void sand_zone__update_sand(sand_zone* this) BANKED {
    _detect_empty_and_similar_chains(this);

#ifndef SKIP_FIND_FAMILIES
    if (_should_check_for_start_to_end_path(this)) {
        _check_for_start_to_end_path(this);
    }
#endif

    uint8_t width = this->width * 8;

    bool* swap = this->needs_update;
    this->needs_update = this->was_updated;
    this->was_updated = swap;

    for (uint8_t x = 0; x < width; x++) {
        this->was_updated[x] = false;

        sand_chain *chain = &this->sand_chains[x];

        while (chain != NULL) {
            _move_next_chain_down(this, x, chain);
            chain = chain->next;
        }
    }

#ifndef SKIP_SLIDE_SAND
    for (uint8_t x = 0; x < width - 1; x++) {
        if (!this->needs_update[x] && !this->needs_update[x + 1]) continue;
        _slide_sand_chain(this, x + 1, x);
    }

    for (uint8_t x = width - 1; x > 0; x--) {
        if (!this->needs_update[x] && !this->needs_update[x - 1]) continue;
        _slide_sand_chain(this, x - 1, x);
    }
#endif

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
    //return bitmap_area__get_color(this->bitmap_area, x, y) > 0;
    sand_chain* chain = &this->sand_chains[x];
    sand_chain* top = sand_chain__get_last_connected(chain);
    uint8_t height = top->y + top->length;
    return y < height;
}

/******* PUBLIC CLASS *******/

sand_zone* sand_zone__new(uint8_t x, uint8_t y, uint8_t width, uint8_t height, ui_lines* lines) BANKED {
    sand_zone* zone = allocate(sizeof(sand_zone));
    zone->x = x;
    zone->y = y;
    zone->width = width;
    zone->height = height;
    zone->_lines = lines;

    zone->bitmap_area = bitmap_area__new(x, y, width, height);

    zone->sand_chains = sand_chain__new_array(zone->width * 8);

    zone->needs_update = allocate_array(zone->width * 8, sizeof(bool));
    zone->was_updated = allocate_array(zone->width * 8, sizeof(bool));

    zone->start_to_end_triggered = true;

    return zone;
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
