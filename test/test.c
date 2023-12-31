#include "test.h"
#include <stdint.h>
#include <stdio.h>
#include <gb/gb.h>

int8_t assert_number = -1;
bool assertion_failed;
uint8_t passed = 0;
uint8_t failed = 0;

void ASSERT(bool is_true) {
    if (assertion_failed) return;
    assert_number++;

    if (!is_true) {
        assertion_failed = true;
    }
}

void _run_test(struct test this) {
    assert_number = -1;
    assertion_failed = false;
    printf("%s ", this.name);
    SWITCH_ROM_MBC1(this.bank);
    this.func();
    if (assertion_failed) {
        printf("FAILED\n");
        printf("ASSERT #%hd\n", assert_number);
        failed++;
    } else {
        printf("PASSED\n");
        passed++;
    }
}

void run_tests(struct test* tests) {
    for (uint8_t i = 0; tests[i].func != NULL; i++) {
        _run_test(tests[i]);
    }

    printf("\nSummary:\n");
    uint8_t total = passed + failed;
    printf(" %u/%u tests passed\n", passed, total);

    printf("\n");
    if (failed == 0) {
        printf("SUCCESS:\n");
        printf(" All unit tests have passed!\n");
    } else {
        printf("FAILED:\n");
        printf(" %u of %u unit tests failed.\n", failed, total);
    }
}