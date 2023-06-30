#include <gb/gb.h>
#include <stdbool.h>
#include "tile_zone.h"

int main(void)
{
    SHOW_BKG;
    struct tile_zone* tz = new_tile_zone(0, 0, 12, 18);

    while(1) {
        // Game main loop processing goes here

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }

    delete_tile_zone(tz);
    return 0;
}
