#include <stdio.h>
#include <gb/gb.h>
#include <stdlib.h>
#include "test.h"

void test_example(void)
{
    void* mem;
    int a, b;

    mem = malloc(10);
    ASSERT(mem != NULL);

    mem = realloc(mem, 20);
    ASSERT(mem != NULL);

    free(mem);
}

const struct test test_list[] = {
    { "example", test_example },
    { NULL, NULL }
};

int main()
{
    SHOW_BKG;
    run_tests(test_list);
    wait_vbl_done();
    return 0;
}