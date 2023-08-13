#include "sand_zone.h"
#include "piece_master.h"
#include "tile_border.h"
#include "text_area.h"
#include "ui_lines.h"
#include "ui_game_over.h"
#include "tile_set.h"
#include "music.h"
#include "global.h"

#include "../res/TILES_Title.h"
#include "../res/MAP_Title.h"

#include <stdbool.h>
#include <rand.h>
#include <gb/gb.h>

void show_title() {
    music__init();

    uint8_t previous_bank = CURRENT_BANK;
    SWITCH_ROM( BANK(TILES_Title) );

    tile_set* set = tile_set__alloc(TILES_TitleLen);
    tile_set__set_data(set, TILES_Title);

    SWITCH_ROM( BANK(MAP_Title) );
    set_bkg_based_tiles(0, 0, MAP_TitleWidth, MAP_TitleHeight, MAP_Title, set->start);

    while(true) {
        music__update();
        uint8_t input = joypad();
        if (input) break;

        wait_vbl_done();
    }

    tile_set__free(set);
    SWITCH_ROM( previous_bank );
}

#include "stdio.h"

void run_game() {
    tile_border* lines_border = tile_border__new(12, 0, 8, 18);
    ui_lines* lines = ui_lines__new(13, 1, 6);

    ui_game_over* game_over = ui_game_over__new(13, 4, 6);

    tile_border* sand_zone_border = tile_border__new(0, 0, 12, 18);
    sand_zone* sand_zone = sand_zone__new(1, 1, 10, 16, lines);
    piece_master* pm = piece_master__new(sand_zone);

    // sand_zone__add_sand(sand_zone, 0, 0, 10, 3);

    while (true) {
        sand_zone__update_sand(sand_zone);
        piece_master__update(pm);
        if (pm->game_over) {
            ui_game_over__show(game_over);
            break;
        }

        wait_vbl_done();
    }

    piece_master__delete(pm);
    sand_zone__delete(sand_zone);
    tile_border__delete(sand_zone_border);

    ui_game_over__delete(game_over);

    ui_lines__delete(lines);
    tile_border__delete(lines_border);
}

int main(void) {
    ENABLE_RAM_MBC1;

    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x8;

    //show_title();

    initrand(DIV_REG);

    global__init();

    run_game();
    return 0;
}
