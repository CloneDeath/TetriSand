#include <gb/gb.h>
#include "test.h"

#include "gameplay/sand_chain_tests.h"
#include "gameplay/sand_chain_list_tests.h"

const struct test test_list[] = {
    /* sand_chain */
    { "sand_chain__excise_chain", test_sand_chain__excise_chain },

    /* sand_chain_list */
    { "sand_chain_list__contains", test__sand_chain_list__contains },
    { NULL, NULL }
};

int main() {
    ENABLE_RAM_MBC1;

    SHOW_BKG;
    run_tests((struct test*)test_list);
    wait_vbl_done();
    return 0;
}