#include "common.h"
#include "psyq/interrupts.h"

// Adapted from https://github.com/sozud/psy-q-decomp

long SetVideoMode(long mode) {
    long prev;

    prev = g_VideoMode;
    g_VideoMode = mode;
    return prev;
}

long GetVideoMode(void) { 
    return g_VideoMode; 
}
