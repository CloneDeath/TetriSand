#pragma once
#include "stdlib.h"
#include "sand_chain.h"

/******* CLASS *******/

sand_chain* sand_memory__alloc();
sand_chain* sand_memory__calloc(size_t count);
void sand_memory__free(sand_chain* pointer);
