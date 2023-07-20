#include <gb/gb.h>
#include "sand_zone.h"
#include "piece_master.h"
#include <stdbool.h>
#include <rand.h>
#include "TileBorder.h"
#include "tile_set.h"
#include "../res/tile-title.h"
#include "../res/map-title.h"
#include "TextArea.h"
#include "global.h"
#include "stdio.h"

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

    struct TileBorder* tb2 = TileBorder.new(12, 0, 8, 18);

    //ta->print_number(ta, 13209);

    while(true) {
        sand_zone__update_sand(tz);
        piece_master__update(pm);

        wait_vbl_done();
    }

    delete_sand_zone(tz);
    TileBorder.delete(tb);
    TileBorder.delete(tb2);
    delete_piece_master(pm);
}

volatile uint8_t* const RAM = 0xA000;

int main(void) {
    ENABLE_RAM_MBC1;

    RAM[0] = 0x38;
    uint8_t x = RAM[0];
    printf("%x", x);
    return 1;

    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x8;

    show_title();

    initrand(DIV_REG);

    Debug.init();

    run_game();
    return 0;
}
