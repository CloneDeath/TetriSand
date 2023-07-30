#pragma once

#include <stdint.h>
#include <stdbool.h>

struct test {
    const char* name;
    void (*func)(void);
    uint8_t bank;
};

void run_tests(struct test* tests);

void ASSERT(bool is_true);
