#include <gb/gb.h>
#include "sand_zone.h"
#include "piece_master.h"
#include <stdbool.h>
#include <rand.h>
#include "TileBorder.h"
#include "tile_set.h"
#include "../res/tile-title.h"
#include "../res/map-title.h"
#include "text_area.h"
#include "global.h"
#include "stdio.h"

void show_title() {
    tile_set* set = tile_set__alloc(47);
    tile_set__set_data(set, TILE_Title);
    set_bkg_based_tiles(0, 0, MAP_TitleWidth, MAP_TitleHeight, MAP_Title, set->start);

    while(true) {
        uint8_t input = joypad();
        if (input) break;

        wait_vbl_done();
    }

    tile_set__free(set);
}

void run_game() {
    struct TileBorder* tb = TileBorder.new(0, 0, 12, 18);
    sand_zone* tz = sand_zone__new(1, 1, 10, 16);
    piece_master* pm = piece_master__new(tz);

    struct TileBorder* tb2 = TileBorder.new(12, 0, 8, 18);

    //ta->print_number(ta, 13209);

    while (true) {
        sand_zone__update_sand(tz);
        piece_master__update(pm);

        wait_vbl_done();
    }

    sand_zone__delete(tz);
    TileBorder.delete(tb);
    TileBorder.delete(tb2);
    piece_master__delete(pm);
}

int main(void) {
    ENABLE_RAM_MBC1;

    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x8;

    show_title();

    initrand(DIV_REG);

    global__init();

    run_game();
    return 0;
}
