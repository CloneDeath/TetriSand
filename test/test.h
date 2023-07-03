#pragma once

#include <stdbool.h>

struct test {
    const char* name;
    void (*func)(void);
};

void run_tests(struct test* tests);

void ASSERT(bool is_true);
