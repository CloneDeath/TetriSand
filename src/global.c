#include "global.h"
#include "text_area.h"
#include "stddef.h"

text_area* global__text = NULL;

void global__init() {
    global__text = text_area__new(13, 1, 6, 16);
}