#include "stdio.h"
#include <gb/gb.h>
#include <stdbool.h>

int main() {
    SHOW_BKG;
    while(true) {
        printf("pass\n");
        wait_vbl_done();
    }
}