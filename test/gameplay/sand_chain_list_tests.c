#pragma bank 255

#include "sand_chain_list_tests.h"

#include "../../src/gameplay/sand_chain_list.h"

#include "../test.h"

#include <gb/gb.h>

BANKREF(test__sand_chain_list)

/******* INSTANCE *******/

void test__sand_chain_list__contains() {
    sand_chain_list* list = sand_chain_list__new();
    sand_chain* a = sand_chain__new(0, 10, 1);
    sand_chain* b = sand_chain__new(0, 10, 2);
    sand_chain* c = sand_chain__new(0, 10, 3);
    {
        sand_chain_list__push_front(list, b, 2);
        sand_chain_list__push_front(list, a, 1);

        ASSERT(sand_chain_list__contains(list, a));
        ASSERT(sand_chain_list__contains(list, b));
        ASSERT(!sand_chain_list__contains(list, c));
    }
    sand_chain__delete(c);
    sand_chain__delete(b);
    sand_chain__delete(a);
    sand_chain_list__delete(list);
}

void test__sand_chain_list__contains_x() {
    sand_chain_list* list = sand_chain_list__new();
    sand_chain* a = sand_chain__new(0, 10, 1);
    sand_chain* b = sand_chain__new(0, 10, 1);
    sand_chain* c = sand_chain__new(0, 10, 1);
    {
        sand_chain_list__push_front(list, b, 2);
        sand_chain_list__push_front(list, a, 1);

        ASSERT(!sand_chain_list__contains_x(list, 0));
        ASSERT(sand_chain_list__contains_x(list, 1));
        ASSERT(sand_chain_list__contains_x(list, 2));
        ASSERT(!sand_chain_list__contains_x(list, 3));
    }
    sand_chain__delete(c);
    sand_chain__delete(b);
    sand_chain__delete(a);
    sand_chain_list__delete(list);
}

void test__sand_chain_list__has_any() {
    /* test has_any works with items */
    {
        sand_chain_list* list1 = sand_chain_list__new();
        sand_chain* a = sand_chain__new(0, 10, 1);
        sand_chain_list__push_front(list1, a, 1);

        ASSERT(sand_chain_list__has_any(list1));

        sand_chain__delete(a);
        sand_chain_list__delete(list1);
    }

    /* has_any returns false with no items */
    {
        sand_chain_list* list2 = sand_chain_list__new();

        ASSERT(!sand_chain_list__has_any(list2));

        sand_chain_list__delete(list2);
    }

    /* has_any returns false if the last item is popped */
    {
        sand_chain_list* list = sand_chain_list__new();
        sand_chain* a = sand_chain__new(0, 10, 1);
        sand_chain_list__push_front(list, a, 1);
        sand_chain_reference* ref = sand_chain_list__pop_front(list);

        ASSERT(!sand_chain_list__has_any(list));

        sand_chain__delete(a);
        sand_chain_list__delete(list);
        sand_chain_reference__delete_single(ref);
    }
}

void test__sand_chain_list__push_front() {
    /* pushing an item in the front makes it the new first item */
    {
        sand_chain_list* list = sand_chain_list__new();
        sand_chain* a = sand_chain__new(0, 10, 1);

        sand_chain_list__push_front(list, a, 1);

        ASSERT(list->_items->chain == a);
        ASSERT(list->_items->x == 1);

        sand_chain__delete(a);
        sand_chain_list__delete(list);
    }
    /* pushing an item in the front makes the previous first item next in the list */
    {
        sand_chain_list* list = sand_chain_list__new();
        sand_chain* a = sand_chain__new(0, 10, 1);
        sand_chain* b = sand_chain__new(0, 10, 1);
        sand_chain_list__push_front(list, a, 1);

        sand_chain_list__push_front(list, b, 2);

        // b
        ASSERT(list->_items->chain == b);
        ASSERT(list->_items->x == 2);
        // a
        ASSERT(list->_items->next->chain == a);
        ASSERT(list->_items->next->x == 1);

        sand_chain__delete(a);
        sand_chain__delete(b);
        sand_chain_list__delete(list);
    }
}

void test__sand_chain_list__push_front_reference() {
    /* if a new reference is pushed front, then it is adopted correctly */
    {
        sand_chain_list* list = sand_chain_list__new();
        sand_chain* x = sand_chain__new(0, 10, 1);
        sand_chain_reference* ref = sand_chain_reference__new(x, 2, NULL);

        sand_chain_list__push_front_reference(list, ref);

        ASSERT(list->_items->chain == x);
        ASSERT(list->_items->x == 2);
        ASSERT(list->_items->next == NULL);

        sand_chain__delete(x);
        sand_chain_list__delete(list);
    }
    /* if a new reference is pushed front, then it correctly links to any existing references */
    {
        sand_chain_list* list = sand_chain_list__new();
        sand_chain* a = sand_chain__new(0, 10, 1);
        sand_chain* b = sand_chain__new(0, 10, 1);
        sand_chain_list__push_front(list, a, 1);
        sand_chain_reference* ref = sand_chain_reference__new(b, 2, NULL);

        sand_chain_list__push_front_reference(list, ref);

        // b
        ASSERT(list->_items->chain == b);
        ASSERT(list->_items->x == 2);
        // a
        ASSERT(list->_items->next->chain == a);
        ASSERT(list->_items->next->x == 1);

        sand_chain__delete(a);
        sand_chain__delete(b);
        sand_chain_list__delete(list);
    }
}

void test__sand_chain_list__pop_front() {
    /* if only one item exists, then popping it makes the list empty */
    {
        sand_chain_list* list = sand_chain_list__new();
        sand_chain* x = sand_chain__new(0, 10, 1);
        sand_chain_list__push_front(list, x, 11);

        sand_chain_reference* ref = sand_chain_list__pop_front(list);

        ASSERT(list->_items == NULL);
        ASSERT(ref->chain == x);
        ASSERT(ref->x == 11);
        ASSERT(ref->next == NULL);

        sand_chain__delete(x);
        sand_chain_list__delete(list);
        sand_chain_reference__delete_single(ref);
    }
    /* when an item is popped, then the next item is made first */
    {
        sand_chain_list* list = sand_chain_list__new();
        sand_chain* x = sand_chain__new(0, 1, 1);
        sand_chain* front = sand_chain__new(0, 1, 1);
        sand_chain_list__push_front(list, front, 1);
        sand_chain_list__push_front(list, x, 12);

        sand_chain_reference* ref = sand_chain_list__pop_front(list);

        ASSERT(list->_items->chain == front);
        ASSERT(list->_items->x == 1);
        ASSERT(list->_items->next == NULL);
        ASSERT(ref->chain == x);
        ASSERT(ref->x == 12);
        ASSERT(ref->next == NULL);

        sand_chain__delete(x);
        sand_chain__delete(front);
        sand_chain_list__delete(list);
        sand_chain_reference__delete_single(ref);
    }
}

void test__sand_chain_list__get_last() {
    /* it returns the last item in the list */
    {
        sand_chain_list* list = sand_chain_list__new();
        sand_chain* a = sand_chain__new(0, 1, 1);
        sand_chain* b = sand_chain__new(0, 1, 1);
        sand_chain* c = sand_chain__new(0, 1, 1);
        sand_chain_list__push_front(list, a, 1);
        sand_chain_list__push_front(list, b, 12);
        sand_chain_list__push_front(list, c, 13);

        sand_chain_reference* last = sand_chain_list__get_last(list);

        ASSERT(last->chain == a);
        ASSERT(last->x == 1);
        ASSERT(last->next == NULL);

        sand_chain__delete(a);
        sand_chain__delete(b);
        sand_chain__delete(c);
        sand_chain_list__delete(list);
    }
    /* if a list is empty, then null is returned */
    {
        sand_chain_list* list = sand_chain_list__new();

        sand_chain_reference* last = sand_chain_list__get_last(list);

        ASSERT(last == NULL);

        sand_chain_list__delete(list);
    }
}

/******* CLASS *******/

void test__sand_chain_list__combine() {
    /* if both lists are empty, it will return a new empty list */
    {
        sand_chain_list* left = sand_chain_list__new();
        sand_chain_list* right = sand_chain_list__new();

        sand_chain_list* combined = sand_chain_list__combine(left, right);

        ASSERT(combined->_items == NULL);

        sand_chain_list__delete(combined);
    }
    /* if the left list is empty, then the right list is returned */
    {
        sand_chain* a = sand_chain__new(0, 1, 1);
        sand_chain_list* left = sand_chain_list__new();
        sand_chain_list* right = sand_chain_list__new();
        sand_chain_list__push_front(right, a, 0);

        sand_chain_list* combined = sand_chain_list__combine(left, right);

        ASSERT(combined == right);
        ASSERT(combined->_items->chain == a);
        ASSERT(combined->_items->x == 0);
        ASSERT(combined->_items->next == NULL);

        sand_chain__delete(a);
        sand_chain_list__delete(combined);
    }
    /* if the right list is empty, then the left list is returned */
    {
        sand_chain* a = sand_chain__new(0, 1, 1);
        sand_chain_list* left = sand_chain_list__new();
        sand_chain_list__push_front(left, a, 0);
        sand_chain_list* right = sand_chain_list__new();

        sand_chain_list* combined = sand_chain_list__combine(left, right);

        ASSERT(combined == left);
        ASSERT(combined->_items->chain == a);
        ASSERT(combined->_items->x == 0);
        ASSERT(combined->_items->next == NULL);

        sand_chain__delete(a);
        sand_chain_list__delete(combined);
    }
    /* if neither list is empty, then the items are combined */
    {
        sand_chain* a = sand_chain__new(0, 1, 1);
        sand_chain* b = sand_chain__new(0, 1, 1);
        sand_chain_list* left = sand_chain_list__new();
        sand_chain_list__push_front(left, a, 0);
        sand_chain_list* right = sand_chain_list__new();
        sand_chain_list__push_front(right, b, 1);

        sand_chain_list* combined = sand_chain_list__combine(left, right);

        ASSERT(combined->_items->chain == a);
        ASSERT(combined->_items->x == 0);
        ASSERT(combined->_items->next->chain == b);
        ASSERT(combined->_items->next->x == 1);
        ASSERT(combined->_items->next->next == NULL);

        sand_chain__delete(a);
        sand_chain__delete(b);
        sand_chain_list__delete(combined);
    }
}
