#include <stdio.h>
#include <gb/gb.h>
#include "test.h"
#include "../src/sand_chain.h"

void excise_chain_works(void) {
    sand_chain* chain = sand_chain__new(0, 10, 3);

    sand_chain* split = sand_chain__excise_chain(chain, 3, 5);

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

    sand_chain__delete(chain);
    sand_chain__delete(split);
}

const struct test test_list[] = {
    { "excise_chain_works", excise_chain_works },
    { NULL, NULL }
};

int main() {
    SHOW_BKG;
    run_tests((struct test*)test_list);
    wait_vbl_done();
    return 0;
}