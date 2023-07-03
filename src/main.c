#include <gb/gb.h>
#include "tile_zone.h"
#include <stdbool.h>

static inline void add_square(struct tile_zone* tz, uint8_t x, uint8_t y, uint8_t size, uint8_t value){
    for (uint8_t i = 0; i < size; i++){
        add_sand(tz, x + i, y, size, value);
    }
}

int main(void)
{
    SHOW_BKG;

    struct tile_zone* tz = new_tile_zone(0, 0, 12, 18);
    /*add_square(tz, 0, 16, 8, 3);
    add_square(tz, 0, 32, 4, 2);
    add_square(tz, 0, 28, 1, 1);
    add_square(tz, 1, 28, 1, 1);

    add_square(tz, 16, 40, 20, 1);*/

    add_square(tz, 0, 3, 20, 3);

    while(true) {
        update_sand(tz);

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }

    // delete_tile_zone(tz);
    // return 0;
}
