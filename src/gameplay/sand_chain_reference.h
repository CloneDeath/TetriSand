#pragma once

#include "sand_chain.h"

typedef struct sand_chain_reference_struct {
    sand_chain* chain;
    uint8_t x;
    struct sand_chain_reference_struct* next;
} sand_chain_reference;

/******* CLASS *******/
sand_chain_reference*   sand_chain_reference__new(sand_chain* chain, uint8_t x, sand_chain_reference* next);
void                    sand_chain_reference__delete_single(sand_chain_reference* this);
void                    sand_chain_reference__delete_all(sand_chain_reference* this);