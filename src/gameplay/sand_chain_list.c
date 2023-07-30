#include "sand_chain_list.h"

#include "../allocate.h"

/******* INSTANCE *******/

#include "stdio.h"

bool sand_chain_list__contains(sand_chain_list* this, sand_chain* item) {
    sand_chain_reference* current = this->_items;
    while (current != NULL) {
        if (current->chain == item) return true;
        current = current->next;
    }
    return false;
}

bool sand_chain_list__contains_x(sand_chain_list* this, uint8_t x) {
    sand_chain_reference* current = this->_items;
    while (current != NULL) {
        if (current->x == x) return true;
        current = current->next;
    }
    return false;
}

bool sand_chain_list__has_any(sand_chain_list* this) {
    return this->_items != NULL;
}

void sand_chain_list__push_front(sand_chain_list* this, sand_chain* item, uint8_t x) {
    this->_items = sand_chain_reference__new(item, x, this->_items);
}

void sand_chain_list__push_front_reference(sand_chain_list* this, sand_chain_reference* ref) {
    ref->next = this->_items;
    this->_items = ref;
}

sand_chain_reference* sand_chain_list__pop_front(sand_chain_list* this) {
    sand_chain_reference* ref = this->_items;
    this->_items = ref->next;
    ref->next = NULL;
    return ref;
}

sand_chain_reference* sand_chain_list__get_last(sand_chain_list* this) {
    if (this->_items == NULL) return NULL;

    sand_chain_reference* current = this->_items;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}

/******* CLASS *******/

sand_chain_list* sand_chain_list__combine(sand_chain_list* this, sand_chain_list* other) {
    sand_chain_reference* last = sand_chain_list__get_last(this);
    if (last == NULL) {
        sand_chain_list__delete(this);
        return other;
    }

    last->next = other->_items;
    other->_items = NULL;
    sand_chain_list__delete(other);
    return this;
}

sand_chain_list* sand_chain_list__new() {
    sand_chain_list* this = allocate(sizeof(sand_chain_list));
    this->_items = NULL;
    return this;
}


sand_chain_list* sand_chain_list__new_array(size_t count) {
    return allocate_array(count, sizeof(sand_chain_list));
}

void sand_chain_list__delete(sand_chain_list* this) {
    sand_chain_reference__delete_all(this->_items);
    free(this);
}

void sand_chain_list__delete_array(sand_chain_list* this, size_t count) {
    for (size_t i = 0; i < count; i++){
        sand_chain_list* list = (this + i);
        sand_chain_reference__delete_all(list->_items);
    }
    free(this);
}