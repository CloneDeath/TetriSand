#pragma once

#include "sand_chain_reference.h"
#include "sand_chain.h"
#include "stdbool.h"

typedef struct sand_chain_list_struct {
    // public:
    uint16_t length;

    // private:
    sand_chain_reference* _first;
    sand_chain_reference* _last;
} sand_chain_list;

/******* INSTANCE *******/

bool                    sand_chain_list__contains(sand_chain_list* this, sand_chain* item);
bool                    sand_chain_list__contains_x(sand_chain_list* this, uint8_t x);
bool                    sand_chain_list__has_any(sand_chain_list* this);
void                    sand_chain_list__push_front(sand_chain_list* this, sand_chain* item, uint8_t x);
void                    sand_chain_list__push_front_reference(sand_chain_list* this, sand_chain_reference* ref);
sand_chain_reference*   sand_chain_list__pop_front(sand_chain_list* this);
sand_chain_reference*   sand_chain_list__get_last(sand_chain_list* this);

/******* CLASS *******/

sand_chain_list*    sand_chain_list__combine(sand_chain_list* this, sand_chain_list* other);

sand_chain_list*    sand_chain_list__new();
sand_chain_list*    sand_chain_list__new_array(size_t count);
void                sand_chain_list__delete(sand_chain_list* this);
void                sand_chain_list__delete_array(sand_chain_list* this, size_t count);