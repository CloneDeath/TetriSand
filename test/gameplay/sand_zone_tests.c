#pragma bank 255

#include "sand_zone_tests.h"
#include "../test.h"

#include "../../src/gameplay/sand_zone.h"

#include <gb/gb.h>

BANKREF(test__sand_zone)

void test__sand_zone__get_connected_chains_in_column() BANKED {
    /* able to get items on the right */
    {
        sand_zone* zone = sand_zone__new(0, 0, 1, 1);
        sand_zone__add_sand(zone, 0, 0, 2, 1);
        sand_zone__add_sand(zone, 1, 0, 2, 1);
        sand_chain* current = zone->sand_chains[0].next;

        sand_chain_list* list = sand_zone__get_connected_chains_in_column(zone, current, 1);

        ASSERT(list->_first->chain->y == 0);
        ASSERT(list->_first->chain->length == 2);
        ASSERT(list->_first->chain->value == 1);
        ASSERT(list->_first->chain->next == NULL);
        ASSERT(list->_first->x == 1);
        ASSERT(list->_first->next == NULL);

        sand_chain_list__delete(list);
        sand_zone__delete(zone);
    }
}