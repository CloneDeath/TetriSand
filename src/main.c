#include <gb/gb.h>
#include "sand_zone.h"
#include "piece_master.h"
#include <stdbool.h>
#include <rand.h>
#include "TileBorder.h"
#include "tile_set.h"
#include "../res/tile-title.h"
#include "../res/map-title.h"

void show_title() {
    struct tile_set* set = alloc_tile_set(47);
    tile_set__set_data(set, TILE_Title);
    set_bkg_based_tiles(0, 0, MAP_TitleWidth, MAP_TitleHeight, MAP_Title, set->start);

    while(true) {
        uint8_t input = joypad();
        if (input) break;

        wait_vbl_done();
    }

    free_tile_set(set);
}

void run_game() {
    struct TileBorder* tb = TileBorder.new(0, 0, 12, 18);
    struct sand_zone* tz = new_sand_zone(1, 1, 10, 16);
    struct piece_master* pm = new_piece_master(tz);

    while(true) {
        sand_zone__update_sand(tz);
        piece_master__update(pm);

        wait_vbl_done();
    }

    delete_sand_zone(tz);
    TileBorder.delete(tb);
    delete_piece_master(pm);
}

int main(void) {
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x8;

    show_title();

    initrand(DIV_REG);

    run_game();
    return 0;
}
