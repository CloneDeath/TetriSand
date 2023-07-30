#include "sand_chain_list_tests.h"

#include "../../src/gameplay/sand_chain_list.h"

#include "../test.h"

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