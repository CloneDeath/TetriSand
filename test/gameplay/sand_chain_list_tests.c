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