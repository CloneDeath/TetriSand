#include "sand_chain.h"
#include "stdlib.h"
#include "stdbool.h"

struct sand_chain*  new_sand_chain(uint8_t y, uint8_t length, uint8_t value) {
    struct sand_chain* chain = malloc(sizeof(struct sand_chain));
    chain->y = y;
    chain->length = length;
    chain->value = value;
    chain->next = NULL;
    return chain;
}

struct sand_chain* chain__add_chain(struct sand_chain* this, uint8_t y, uint8_t length, uint8_t value) {
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