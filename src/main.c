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
    add_square(tz, 0, 32, 4, 2);
    add_square(tz, 0, 42, 4, 2);

    //add_square(tz, 39, 10, 5, 1);

    //add_square(tz, 40, 3, 3, 3);

    // single thread case
    // add_sand(tz, 40, 3, 30, 2);
    // add_sand(tz, 25, 60, 30, 2);

    uint8_t previous = joypad();
    uint8_t current = joypad();

    while(true) {
        previous = current;
        current = joypad();

        if (current & J_A && !(previous & J_A)) {
            add_square(tz, 30, 100, 20, 3);
        }
        if (current & J_B && !(previous & J_B)) {
            add_square(tz, 30, 100, 20, 2);
        }

        update_sand(tz);


        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }

    // delete_tile_zone(tz);
    // return 0;
}
