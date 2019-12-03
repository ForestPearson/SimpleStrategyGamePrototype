#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <random>
#include "ENGINE/bahamut.h"

static inline 
const char* format_text(const char* text, ...) {
    static char buffer[MAX_FORMAT_TEXT_SIZE];

    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);

    return buffer;
}

static inline
i32 random_int(i32 min, i32 max) {
    static std::random_device rd;
    static std::mt19937 mt(rd());

    std::uniform_int_distribution<> dist(0, max - min);
    return dist(mt) + min;
}

static inline
void clamp(i32& input, i32 min, i32 max) {
    if (input > max) input = max;
    if (input < min) input = min;
}

#endif
