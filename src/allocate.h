#pragma once

#include <stddef.h>

void* allocate(size_t amount);
void* allocate_array(size_t count, size_t size);