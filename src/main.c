#include <gb/gb.h>
#include "tile_zone.h"
#include <stdbool.h>
#include "../res/tetromino.h"

static inline void add_square(struct tile_zone* tz, uint8_t x, uint8_t y, uint8_t size, uint8_t value){
    for (uint8_t i = 0; i < size; i++){
        add_sand(tz, x + i, y, size, value);
    }
}

int main(void)
{
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x8;

    struct tile_zone* tz = new_tile_zone(0, 0, 12, 18);
    add_square(tz, 0, 32, 4, 2);
    add_square(tz, 0, 42, 4, 2);

    set_sprite_data(0, 4, Tetromino);
    set_sprite_tile(0, 3);
    move_sprite(0, 75, 75);

    uint8_t previous;
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
