#pragma once
#include "TextArea.h"

extern struct DebugClass {
    struct TextArea* text;

    void (*init)();
} Debug;
