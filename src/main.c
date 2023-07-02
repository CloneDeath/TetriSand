#include <gb/gb.h>
#include "tile_zone.h"
#include <stdbool.h>

int main(void)
{
    SHOW_BKG;

    struct tile_zone* tz = new_tile_zone(0, 0, 12, 18);

    add_sand(tz, 0, 16, 8, 3);
    add_sand(tz, 1, 16, 8, 3);
    add_sand(tz, 2, 16, 8, 3);
    add_sand(tz, 3, 16, 8, 3);
    add_sand(tz, 4, 16, 8, 3);
    add_sand(tz, 5, 16, 8, 3);
    add_sand(tz, 6, 16, 8, 3);
    add_sand(tz, 7, 16, 8, 3);
    add_sand(tz, 8, 16, 8, 3);

    add_sand(tz, 0, 24, 4, 2);
    add_sand(tz, 1, 24, 4, 2);
    add_sand(tz, 2, 24, 4, 2);
    add_sand(tz, 3, 24, 4, 2);

    while(true) {
        update_sand(tz);

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }

    delete_tile_zone(tz);
    return 0;
}
