#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include "../res/border.h"

int main(void)
{
    SHOW_BKG;
    set_bkg_data(1, 8, BorderTiles);
    set_bkg_tile_xy(0, 0, 1);
    set_bkg_tile_xy(1, 0, 2);
    set_bkg_tile_xy(2, 0, 3);
    set_bkg_tile_xy(0, 1, 4);
    set_bkg_tile_xy(2, 1, 5);
    set_bkg_tile_xy(0, 2, 6);
    set_bkg_tile_xy(1, 2, 7);
    set_bkg_tile_xy(2, 2, 8);

    // Loop forever
    while(1) {
        // Game main loop processing goes here

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
    return 0;
}
