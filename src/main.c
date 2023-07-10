#include <gb/gb.h>
#include "sand_zone.h"
#include "piece_master.h"
#include <stdbool.h>
#include <rand.h>
#include "tile_border.h"

int main(void) {
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x8;

    initrand(DIV_REG);

    struct tile_border* tb = new_tile_border(0, 0, 12, 18);
    struct sand_zone* tz = new_sand_zone(1, 1, 10, 16);
    struct piece_master* pm = new_piece_master(tz);

    while(true) {
        sand_zone__update_sand(tz);
        piece_master__update(pm);

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }

    // delete_sand_zone(tz);
    // return 0;
}
