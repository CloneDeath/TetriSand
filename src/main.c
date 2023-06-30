#include <gb/gb.h>
#include "tile_zone.h"

int main(void)
{
    SHOW_BKG;

    struct tile_zone* tz = new_tile_zone(0, 0, 12, 18);

    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t z = 3;

    while(1) {
        // Game main loop processing goes here
        set_sand(tz, x, y, z);
        x++;
        if (x >= 80){
            x = 0;
            y++;
        }
        //z = (z+1) % 4;

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }

    delete_tile_zone(tz);
    return 0;
}
