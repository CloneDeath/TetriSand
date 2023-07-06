#include <gb/gb.h>
#include "tile_zone.h"
#include "piece_master.h"
#include <stdbool.h>
#include <rand.h>

int main(void) {
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x8;

    initrand(DIV_REG);

    struct tile_zone* tz = new_tile_zone(0, 0, 12, 18);
    struct piece_master* pm = new_piece_master(tz);

    while(true) {
        update_sand(tz);
        piece_master__update(pm);

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }

    // delete_tile_zone(tz);
    // return 0;
}
