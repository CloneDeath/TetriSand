#include <stdio.h>
#include <gb/gb.h>
#include "test.h"
#include "../src/sand_chain.h"

void test_example(void)
{
    struct sand_chain* chain = new_sand_chain(0, 10, 3);

    struct sand_chain* split = sand_chain__excise_chain(chain, 3, 5);

    ASSERT(chain->y == 0);
    ASSERT(chain->length == 3);
    ASSERT(chain->value == 3);

    ASSERT(chain->next != split);
    ASSERT(chain->next->y == 8);
    ASSERT(chain->next->length == 2);
    ASSERT(chain->next->value == 3);
    ASSERT(chain->next->next == NULL);

    ASSERT(split != NULL);
    ASSERT(split->y == 3);
    ASSERT(split->length == 5);
    ASSERT(split->value == 3);
    ASSERT(split->next == NULL);

    free_sand_chain(chain);
    free_sand_chain(split);
}

const struct test test_list[] = {
    { "example", test_example },
    { NULL, NULL }
};

int main()
{
    SHOW_BKG;
    run_tests(test_list);
    wait_vbl_done();
    return 0;
}