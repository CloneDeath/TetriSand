#include "sand_chain.h"
#include "stdlib.h"

struct sand_chain*  new_sand_chain(uint8_t y, uint8_t length, uint8_t value) {
    struct sand_chain* chain = malloc(sizeof(struct sand_chain));
    chain->y = y;
    chain->length = length;
    chain->value = value;
    chain->next = NULL;
    return chain;
}