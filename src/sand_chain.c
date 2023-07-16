#include "sand_chain.h"
#include <stdlib.h>
#include <stdbool.h>
#include "allocate.h"
#include <stdio.h>

struct sand_chain* sand_chain__add_chain(struct sand_chain* this, uint8_t y, uint8_t length, uint8_t value) {
    struct sand_chain* current = this;
    while (true) {
        // if y is less than the current link
        if (y < current->y) {
            // make a copy of current
            struct sand_chain* new = SandChain.copy(current);

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
            current->next = SandChain.new(y, length, value);
            return current->next;
        }
        current = current->next;
    }
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
    if (this->next == NULL) {
        return 255 - this->y - this->length;
    }
    if (this->next->y < this->y + this->length) {
        return 127;
    }
    return this->next->y - (this->y + this->length);
}

uint8_t sand_chain__get_connected_length(struct sand_chain* this) {
    struct sand_chain* last = sand_chain__get_last_connected(this);
    return last->y + last->length - this->y;
}


struct sand_chain* _split(struct sand_chain* this, uint8_t split_after) {
    struct sand_chain* next = SandChain.new(this->y + split_after, this->length - split_after, this->value);
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

void sand_chain__try_to_combine(struct sand_chain *this) {
    struct sand_chain* next = this->next;
    if (next == NULL) return;
    if (this->value != next->value) return;
    if (this->y + this->length < next->y) return;

    this->length += next->length;
    this->next = next->next;
    next->next = NULL;
    SandChain.delete(next);
}

/******* CLASS *******/

static struct sand_chain* new(uint8_t y, uint8_t length, uint8_t value) {
    struct sand_chain* chain = allocate(sizeof(struct sand_chain));
    chain->y = y;
    chain->length = length;
    chain->value = value;
    chain->next = NULL;
    return chain;
}

static struct sand_chain* new_array(size_t count) {
    struct sand_chain* chain = allocate_array(count, sizeof(struct sand_chain));
    return chain;
}

static struct sand_chain* copy(struct sand_chain* original) {
    struct sand_chain* chain = allocate(sizeof(struct sand_chain));
    chain->y = original->y;
    chain->length = original->length;
    chain->value = original->value;
    chain->next = original->next;
    return chain;
}

static void delete(struct sand_chain* this) {
    struct sand_chain* current = this;
    while (current != NULL) {
        struct sand_chain* next = current->next;
        free(current);
        current = next;
    }
}

const struct SandChainClass SandChain = {
    .new=&new,
    .new_array=&new_array,
    .copy=&copy,
    .delete=&delete
};