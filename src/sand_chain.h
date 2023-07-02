#pragma once

#include "stdint.h"

struct sand_chain {
    uint8_t y;
    uint8_t length;
    uint8_t value;
    struct sand_chain* next;
};