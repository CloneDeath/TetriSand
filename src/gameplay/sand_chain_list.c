#include "sand_chain_list.h"

#include "../engine/allocate.h"

/******* INSTANCE *******/

#include "stdio.h"

bool sand_chain_list__contains(sand_chain_list* this, sand_chain* item) {
    sand_chain_reference* current = this->_first;
    while (current != NULL) {
        if (current->chain == item) return true;
        current = current->next;
    }
    return false;
}

bool sand_chain_list__contains_x(sand_chain_list* this, uint8_t x) {
    sand_chain_reference* current = this->_first;
    while (current != NULL) {
        if (current->x == x) return true;
        current = current->next;
    }
    return false;
}

bool sand_chain_list__has_any(sand_chain_list* this) {
    return this->_first != NULL;
}

void sand_chain_list__push_front(sand_chain_list* this, sand_chain* item, uint8_t x) {
    this->_first = sand_chain_reference__new(item, x, this->_first);
    if (this->_last == NULL) {
        this->_last = this->_first;
    }
}

void sand_chain_list__push_front_reference(sand_chain_list* this, sand_chain_reference* ref) {
    ref->next = this->_first;
    this->_first = ref;
    if (this->_last == NULL) {
        this->_last = this->_first;
    }
}

sand_chain_reference* sand_chain_list__pop_front(sand_chain_list* this) {
    sand_chain_reference* ref = this->_first;
    this->_first = ref->next;
    ref->next = NULL;
    if (this->_first == NULL) {
        this->_last = NULL;
    }
    return ref;
}

sand_chain_reference* sand_chain_list__get_last(sand_chain_list* this) {
    return this->_last;
}

/******* CLASS *******/

sand_chain_list* sand_chain_list__combine(sand_chain_list* this, sand_chain_list* other) {
    sand_chain_reference* last = sand_chain_list__get_last(this);
    if (last == NULL) {
        sand_chain_list__delete(this);
        return other;
    }

    last->next = other->_first;
    this->_last = other->_last;
    other->_first = NULL;
    other->_last = NULL;
    sand_chain_list__delete(other);
    return this;
}

sand_chain_list* sand_chain_list__new() {
    sand_chain_list* this = allocate(sizeof(sand_chain_list));
    this->_first = NULL;
    this->_last = NULL;
    return this;
}


sand_chain_list* sand_chain_list__new_array(size_t count) {
    return allocate_array(count, sizeof(sand_chain_list));
}

void sand_chain_list__delete(sand_chain_list* this) {
    sand_chain_reference__delete_all(this->_first);
    free(this);
}

void sand_chain_list__delete_array(sand_chain_list* this, size_t count) {
    for (size_t i = 0; i < count; i++){
        sand_chain_list* list = (this + i);
        sand_chain_reference__delete_all(list->_first);
    }
    free(this);
}