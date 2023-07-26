#include <gb/gb.h>
#include "gameplay/sand_zone.h"
#include "gameplay/piece_master.h"
#include <stdbool.h>
#include <rand.h>

#include "../res/TITLE/TILES_Title.h"
#include "../res/TITLE/MAP_Title.h"

#include "tile_border.h"
#include "tile_set.h"
#include "text_area.h"
#include "global.h"

void show_title() {
    uint8_t previous_bank = CURRENT_BANK;
    SWITCH_ROM( BANK(TILES_Title) );

    tile_set* set = tile_set__alloc(47);
    tile_set__set_data(set, TILES_Title);

    SWITCH_ROM( BANK(MAP_Title) );
    set_bkg_based_tiles(0, 0, MAP_TitleWidth, MAP_TitleHeight, MAP_Title, set->start);

    while(true) {
        uint8_t input = joypad();
        if (input) break;

        wait_vbl_done();
    }

    tile_set__free(set);
    SWITCH_ROM( previous_bank );
}

void run_game() {
    tile_border* tb = tile_border__new(0, 0, 12, 18);
    sand_zone* tz = sand_zone__new(1, 1, 10, 16);
    piece_master* pm = piece_master__new(tz);

    tile_border* tb2 = tile_border__new(12, 0, 8, 18);

    //ta->print_number(ta, 13209);

    while (true) {
        sand_zone__update_sand(tz);
        piece_master__update(pm);

        wait_vbl_done();
    }

    // sand_zone__delete(tz);
    // tile_border__delete(tb);
    // tile_border__delete(tb2);
    // piece_master__delete(pm);
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
