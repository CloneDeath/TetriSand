#include <gb/gb.h>
#include "test.h"

#include "gameplay/sand_chain_tests.h"
#include "gameplay/sand_chain_list_tests.h"
#include "gameplay/sand_zone_tests.h"

const struct test test_list[] = {
    /* sand_chain */
    { "sand_chain__excise_chain", test_sand_chain__excise_chain, 0 },

    /* sand_chain_list */
    { "sand_chain_list__contains", test__sand_chain_list__contains, BANK(test__sand_chain_list)},
    { "sand_chain_list__contains_x", test__sand_chain_list__contains_x, BANK(test__sand_chain_list) },
    { "sand_chain_list__has_any", test__sand_chain_list__has_any, BANK(test__sand_chain_list) },
    { "sand_chain_list__push_front", test__sand_chain_list__push_front, BANK(test__sand_chain_list) },
    { "sand_chain_list__push_front_reference", test__sand_chain_list__push_front_reference, BANK(test__sand_chain_list) },
    { "sand_chain_list__pop_front", test__sand_chain_list__pop_front, BANK(test__sand_chain_list) },
    { "sand_chain_list__get_last", test__sand_chain_list__get_last, BANK(test__sand_chain_list) },

    { "sand_chain_list__combine", test__sand_chain_list__combine, BANK(test__sand_chain_list) },

    /* sand_zone */
    { "sand_zone__get_connected_chains_in_column", test__sand_zone__get_connected_chains_in_column, BANK(test__sand_zone) },

    { NULL, NULL }
};

int main() {
    ENABLE_RAM_MBC1;

    SHOW_BKG;
    run_tests((struct test*)test_list);
    wait_vbl_done();
    return 0;
}