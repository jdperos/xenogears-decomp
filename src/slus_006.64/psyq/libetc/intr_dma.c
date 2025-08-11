#include "common.h"
#include "psyq/interrupts.h"

// Adapted from https://github.com/sozud/psy-q-decomp

void trapIntrDMA(void);
void setIntrDMA(unsigned int index, VoidCallback_t callback);
static void memclrIntrDMA(void(**callbacks)(void), unsigned int numCallbacks);

void* startIntrDMA(void) {
    memclrIntrDMA(&g_DmaInterruptCallbacks, MAX_INTERRUPT_CALLBACKS);
    *g_pDMA_DICR = 0;
    InterruptCallback(3, &trapIntrDMA);
    return &setIntrDMA;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", trapIntrDMA);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", setIntrDMA);

static void memclrIntrDMA(void(**callbacks)(void), unsigned int numCallbacks) {
    while (numCallbacks--) {
        *callbacks++ = NULL;
    }
}