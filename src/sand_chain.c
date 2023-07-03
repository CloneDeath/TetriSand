#include "sand_chain.h"
#include "stdlib.h"
#include "stdbool.h"

struct sand_chain* new_sand_chain(uint8_t y, uint8_t length, uint8_t value) {
    struct sand_chain* chain = malloc(sizeof(struct sand_chain));
    chain->y = y;
    chain->length = length;
    chain->value = value;
    chain->next = NULL;
    return chain;
}

struct sand_chain* copy_sand_chain(struct sand_chain* copy) {
    struct sand_chain* chain = malloc(sizeof(struct sand_chain));
    chain->y = copy->y;
    chain->length = copy->length;
    chain->value = copy->value;
    chain->next = copy->next;
    return chain;
}

void free_sand_chain(struct sand_chain* this) {
    struct sand_chain* current = this;
    while (current != NULL) {
        struct sand_chain* next = current->next;
        free(current);
        current = next;
    }
}

struct sand_chain* sand_chain__add_chain(struct sand_chain* this, uint8_t y, uint8_t length, uint8_t value) {
    if (this->length == 0) {
        this->length = length;
        this->value = value;
        this->y = y;
        return this;
    }
    struct sand_chain* current = this;
    while (true) {
        // if y is less than the current link
        if (y < current->y) {
            // make a copy of current
            struct sand_chain* new = new_sand_chain(current->y, current->length, current->value);
            new->next = current->next;

            // and push it to the next chain
            current->next = new;

            // set current to the new chain
            current->y = y;
            current->length = length;
            current->value = value;
            return current;
        }

        // if we overlap with an existing chain, push it to above the chain
        if (y < current->y + current->length) {
            y = current->y + current->length;
        }

        if (!current->next) {
            current->next = new_sand_chain(y, length, value);
            return current->next;
        }

        current = current->next;
    }
    return NULL;
}

struct sand_chain* sand_chain__get_last_connected(struct sand_chain* this) {
    struct sand_chain* previous = this;
    struct sand_chain* current = this->next;
    while (current != NULL) {
        if (current->y > previous->y + previous->length) {
            return previous;
        }
        previous = current;
        current = current->next;
    }
    return previous;
}

uint8_t sand_chain__get_gap_above(struct sand_chain* this) {
    if (this->next == NULL) return 127;
    return this->next->y - (this->y + this->length);
}

uint8_t sand_chain__get_connected_length(struct sand_chain* this) {
    struct sand_chain* last = sand_chain__get_last_connected(this);
    return last->y + last->length - this->y;
}


struct sand_chain* _split(struct sand_chain* this, uint8_t split_after) {
    struct sand_chain* next = new_sand_chain(this->y + split_after, this->length - split_after, this->value);
    next->next = this->next;
    this->next = NULL;
    this->length = split_after;
    return next;
}

struct sand_chain* sand_chain__excise_chain(struct sand_chain* this, uint8_t from, uint8_t length) {
    // find first chain to excise from
    struct sand_chain* first_chain = this;
    while (first_chain != NULL) {
        if (first_chain->y <= from && first_chain->y + first_chain->length > from) break;
        first_chain = first_chain->next;
    }

    uint8_t split_after = from - first_chain->y;
    struct sand_chain* split_chain = _split(first_chain, split_after);
    struct sand_chain* current = split_chain;

    uint8_t total = current->length;
    while (total < length) {
        if (current->next == NULL) break;
        current = current->next;
        total += current->length;
    }
    if (total > length) {
        uint8_t extra_length = total - length;
        struct sand_chain* split_last = _split(current, current->length - extra_length);
        first_chain->next = split_last;
        return split_chain;
    } else {
        first_chain->next = current->next;
        current->next = NULL;
        return split_chain;
    }
}