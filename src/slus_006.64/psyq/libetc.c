#include "common.h"

/*

#include "psyq/interrupts.h"

void setIntrVSync(u32 index, VoidCallback_t callback) {
    if (callback != g_VsyncInterruptCallbacks[index]) {
        g_VsyncInterruptCallbacks[index] = callback;
    }
}

void memclrIntrVSync(void(**callbacks)(void), unsigned int numCallbacks) {
    while (numCallbacks--) {
        *callbacks++ = NULL;
    }
}

/*
Matches on decomp.me, but the volatile globals seems to cause issues when linking.
*/
/*
void trapIntrVSync(void) {
    int i;

    g_VsyncInterruptCount++;
    for (i = 0; i < MAX_INTERRUPT_CALLBACKS; i++) {
        if (g_VsyncInterruptCallbacks[i] != NULL) {
            g_VsyncInterruptCallbacks[i]();
        }
    }
}

SetVsyncIntrCallback_t startIntrVSync() {
    *g_pTMR_HRETRACE_MODE = 0x107;
    g_VsyncInterruptCount = 0;
    ClearVsyncInterruptCallbacks(g_VsyncInterruptCallbacks, 8);
    SetInterruptHandler(0, VsyncInterruptHandler);
    return &SetVsyncInterruptCallbackByIndex;
}

*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", Vsync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", v_wait);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", ChangeClearRCnt);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", ResetCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", InterruptCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B7A0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B7D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B804);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B834);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B864);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B894);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B8A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B8BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", startIntr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", trapIntr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", setIntr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", stopIntr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", restartIntr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", memclrIntr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", setjmp);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004BE8C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004BED0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", startIntrVSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", trapIntrVSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", setIntrVSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", memclrIntrVSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", startIntrDMA);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", trapIntrDMA);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", setIntrDMA);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", memclrIntrDMA);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", SetVideoMode);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", GetVideoMode);
