#include "global.h"
#include "TextArea.h"

static void init() {
    Debug.text = TextArea.new(13, 1, 6, 16);
}

struct DebugClass Debug = {
    .text=0,
    .init=&init
};