#include "sand_chain_tests.h"

#include "test.h"

#include "../src/sand_chain.h"

#include "stdio.h"

void test_sand_chain__excise_chain() {
    sand_chain* chain = sand_chain__new(0, 10, 3);

    sand_chain* split = sand_chain__excise_chain(chain, 3, 5);

    ASSERT(chain->is_free == 0);
    ASSERT(chain->y == 0);
    ASSERT(chain->length == 3);
    ASSERT(chain->value == 3);

    ASSERT(chain->next != split);
    ASSERT(chain->next->is_free == 0);
    ASSERT(chain->next->y == 8);
    ASSERT(chain->next->length == 2);
    ASSERT(chain->next->value == 3);
    ASSERT(chain->next->next == NULL);

    ASSERT(split != NULL);
    ASSERT(split->is_free == 0);
    ASSERT(split->y == 3);
    ASSERT(split->length == 5);
    ASSERT(split->value == 3);
    ASSERT(split->next == NULL);

    sand_chain__delete(chain);
    sand_chain__delete(split);
}