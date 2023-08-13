#include "sand_chain_reference.h"

#include "allocate.h"
#include "sand_chain.h"

uint16_t total = 0;

/******* CLASS *******/

sand_chain_reference* sand_chain_reference__new(sand_chain* chain, uint8_t x, sand_chain_reference* next) {
    total += 1;
    sand_chain_reference* this = allocate(sizeof(sand_chain_reference));
    this->chain = chain;
    this->next = next;
    this->x = x;
    return this;
}

void sand_chain_reference__delete_single(sand_chain_reference* this) {
    total -= 1;
    free(this);
}

void sand_chain_reference__delete_all(sand_chain_reference* this) {
    sand_chain_reference* next = this;
    while (next != NULL) {
        sand_chain_reference* current = next;
        next = current->next;
        sand_chain_reference__delete_single(current);
    }
}