#include "sand_chain.h"
#include "../engine/sand_memory.h"
#include "../global.h"
#include "../ui/text_area.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

/******* PRIVATE INSTANCE *******/

sand_chain* _split(sand_chain* this, uint8_t split_after) {
    if (split_after == 0) {
        return this;
    }
    sand_chain* next = sand_chain__new(this->y + split_after, this->length - split_after, this->value);
    next->next = this->next;
    this->next = NULL;
    this->length = split_after;
    return next;
}

/******* PUBLIC INSTANCE *******/

sand_chain* sand_chain__add_chain(sand_chain* this, uint8_t y, uint8_t length, uint8_t value) {
    sand_chain* current = this;
    while (true) {
        // if y is less than the current link
        if (y < current->y) {
            // make a copy of current
            sand_chain* new = sand_chain__copy(current);

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
            current->next = sand_chain__new(y, length, value);
            return current->next;
        }
        current = current->next;
    }
}

sand_chain* sand_chain__get_last_connected(sand_chain* this) {
    sand_chain* previous = this;
    sand_chain* current = this->next;
    while (current != NULL) {
        if (current->y > previous->y + previous->length) {
            return previous;
        }
        previous = current;
        current = current->next;
    }
    return previous;
}

uint8_t sand_chain__get_gap_above(sand_chain* this) {
    if (this->next == NULL) {
        return 255 - this->y - this->length;
    }
    if (this->next->y < this->y + this->length) {
        printf("NEXT CHAIN COLLIDES WITH CURRENT CHAIN");
        exit(-1);
    }
    return this->next->y - (this->y + this->length);
}

uint8_t sand_chain__get_connected_length(sand_chain* this) {
    sand_chain* last = sand_chain__get_last_connected(this);
    return last->y + last->length - this->y;
}

sand_chain* sand_chain__excise_chain(sand_chain* this, uint8_t from, uint8_t length) {
    // find first chain to excise from
    sand_chain* prev_chain = this;
    sand_chain* first_chain = this->next;
    while (first_chain != NULL) {
        if (first_chain->y <= from && first_chain->y + first_chain->length > from) {
            break;
        }
        first_chain = first_chain->next;
    }

    uint8_t split_after = from - first_chain->y;
    sand_chain* split_chain = _split(first_chain, split_after);
    sand_chain* current = split_chain;
    if (split_after == 0) {
        first_chain = prev_chain;
    }

    uint8_t total = current->length;
    while (total < length) {
        if (current->next == NULL) break;
        current = current->next;
        total += current->length;
    }
    if (total > length) {
        uint8_t extra_length = total - length;
        sand_chain* split_last = _split(current, current->length - extra_length);
        first_chain->next = split_last;
        return split_chain;
    } else {
        first_chain->next = current->next;
        current->next = NULL;
        return split_chain;
    }
}

void sand_chain__try_to_combine(sand_chain *this) {
    sand_chain* next = this->next;
    while (next != NULL && next->length == 0) {
        this->next = next->next;
        next->next = NULL;
        sand_chain__delete(next);
        next = this->next;
    }
    if (next == NULL) return;
    if (this->value != next->value) return;
    if (this->y + this->length < next->y) return;

    this->length += next->length;
    this->next = next->next;
    next->next = NULL;
    this->family = 0;
    sand_chain__delete(next);
}

int8_t sand_chain__get_adjacency(sand_chain* this, sand_chain* other) {
    if (this->length == 0) {
        return -1;
    }
    if (other->length == 0) {
        return -1;
    }

    uint8_t start = this->y;
    uint8_t end = start + this->length - 1;

    if (other->y > end + 1) {
        return 1;
    }
    if (other->y + other->length < start - 1) {
        return -1;
    }
    return 0;
}

/******* CLASS *******/
sand_chain* sand_chain__new(uint8_t y, uint8_t length, uint8_t value) {
    sand_chain* chain = sand_memory__alloc();
    chain->y = y;
    chain->length = length;
    chain->value = value;
    chain->family = 0;
    chain->next = NULL;
    return chain;
}

sand_chain* sand_chain__new_array(size_t count) {
    sand_chain* chain = sand_memory__calloc(count);
    return chain;
}

sand_chain* sand_chain__copy(sand_chain* original) {
    sand_chain* chain = sand_memory__alloc();
    chain->y = original->y;
    chain->length = original->length;
    chain->value = original->value;
    chain->next = original->next;
    chain->family = 0;
    return chain;
}

void sand_chain__delete(sand_chain* this) {
    sand_chain* current = this;
    while (current != NULL) {
        sand_chain* next = current->next;
        sand_memory__free(current);
        current = next;
    }
}