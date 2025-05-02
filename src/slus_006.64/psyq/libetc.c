#include "common.h"

#include "psyq/interrupts.h"

// Adapted from https://github.com/sozud/psy-q-decomp

void trapIntrVSync(void);
void setIntrVSync(unsigned int index, VoidCallback_t callback);
void memclrIntrVSync(void(**callbacks)(void), unsigned int numCallbacks);

static VoidCallback_t setIntr(int index, VoidCallback_t fn);


int setjmp(int*);
SetVsyncIntrCallback_t startIntrVSync();
long long startIntrDMA();

void ChangeClearRCnt(int, int);

/*
static InterruptEnvironment_t g_InterruptEnvironment = {
    0,
    0,
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    0,
    0,
    0,
};

static InterruptControl_t g_InterruptControl = {
    "$$Id: intr.c,v 1.76 1997/02/12 12:45:05 makoto Exp $",
    0,
    setIntr,
    startIntr,
    stopIntr,
    0,
    restartIntr,
    &g_InterruptEnvironment
};
*/

int Vsync(int mode) {
    int nGpuStatusReg;
    int nResult;
    int nMax;
    int nWaitCount;

    nGpuStatusReg = *g_pGPUSTAT;
    nResult = (*g_pTMR_HRETRACE_VAL - g_HsyncInterruptCount) & 0xFFFF;
    
    if (mode < 0)
        return g_VsyncInterruptCount;
    
    if (mode == 1)
        return nResult;
    
    if (mode > 0) {
        nWaitCount = 1;
        nWaitCount = (g_VsyncPrevInterruptCount - nWaitCount) + mode;
    } else {
        nWaitCount = g_VsyncPrevInterruptCount;
    }
    
    nMax = 0;
    if (mode > 0)
        nMax = mode - 1;
    
    v_wait(nWaitCount, nMax);
    nGpuStatusReg = *g_pGPUSTAT;
    v_wait(g_VsyncInterruptCount + 1, 1);
    if ((nGpuStatusReg & 0x400000) && ((nGpuStatusReg ^ *g_pGPUSTAT) >= 0)) {
        do {} while (!((nGpuStatusReg ^ *g_pGPUSTAT) & 0x80000000));
    }
    g_VsyncPrevInterruptCount = g_VsyncInterruptCount;
    g_HsyncInterruptCount = *g_pTMR_HRETRACE_VAL;
    return nResult;
}

extern char D_800193C4; // "VSync: timeout\n"
static void v_wait(int count, int max) {
    volatile int timeout[2];

    timeout[0] = max << 0xF;
    while (g_VsyncInterruptCount < count) {
        if (!timeout[0]--) {
            puts(&D_800193C4);
            ChangeClearPAD(0);
            ChangeClearRCnt(3, 0);
            return;
        }
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", ChangeClearRCnt);

void* ResetCallback(void) { 
    return g_pInterruptControl->intrResetHandler();
}

void InterruptCallback(unsigned int irq, VoidCallback_t fn) {
    g_pInterruptControl->setIntrHandler(irq, fn);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B7A0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B7D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B804);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B834);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B864);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B894);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B8A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004B8BC);

void* startIntr() {
    if (g_InterruptEnvironment.interruptsInitialized)
        return NULL;
    
    *g_pI_STAT = *g_pI_MASK = 0;
    *g_pDMA_DPCR = 0x33333333;
    memclrIntr(&g_InterruptEnvironment, sizeof(g_InterruptEnvironment) / sizeof(int));
    if (setjmp(g_InterruptEnvironment.buf))
        trapIntr();
    
    g_InterruptEnvironment.buf[JB_SP] = (int)&g_InterruptEnvironment.stack[1004];
    HookEntryInt((u_short*)g_InterruptEnvironment.buf);
    g_InterruptEnvironment.interruptsInitialized = 1;
    g_pInterruptControl->setVsyncIntrCallback = startIntrVSync();
    g_pInterruptControl->setDMAIntrCallback = startIntrDMA();
    func_8004BED8();
    ExitCriticalSection();
    return &g_InterruptEnvironment;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", trapIntr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", setIntr);

void* stopIntr() {
    if (!g_InterruptEnvironment.interruptsInitialized)
        return NULL;

    EnterCriticalSection();
    g_InterruptEnvironment.savedMask = *g_pI_MASK;
    g_InterruptEnvironment.savedPcr = *g_pDMA_DPCR;
    *g_pI_STAT = *g_pI_MASK = 0;
    *g_pDMA_DPCR &= 0x77777777;
    ResetEntryInt();
    g_InterruptEnvironment.interruptsInitialized = 0;
    return &g_InterruptEnvironment;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", restartIntr);

static void memclrIntr(void(**callbacks)(void), unsigned int numCallbacks) {
    while (numCallbacks--) {
        *callbacks++ = NULL;
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", setjmp);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", longjmp);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004BEC0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004BED0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", func_8004BED8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", ReturnFromException);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", ResetEntryInt);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", HookEntryInt);

SetVsyncIntrCallback_t startIntrVSync() {
    *g_pTMR_HRETRACE_MODE = 0x107;
    g_VsyncInterruptCount = 0;
    memclrIntrVSync(g_VsyncInterruptCallbacks, 8);
    InterruptCallback(0, trapIntrVSync);
    return &setIntrVSync;
}

static void trapIntrVSync(void) {
    int i;

    g_VsyncInterruptCount++;
    for (i = 0; i < MAX_INTERRUPT_CALLBACKS; i++) {
        if (g_VsyncInterruptCallbacks[i]) {
            g_VsyncInterruptCallbacks[i]();
        }
    }
}

static void setIntrVSync(unsigned int index, VoidCallback_t callback) {
    if (callback != g_VsyncInterruptCallbacks[index]) {
        g_VsyncInterruptCallbacks[index] = callback;
    }
}

static void memclrIntrVSync(void(**callbacks)(void), unsigned int numCallbacks) {
    while (numCallbacks--) {
        *callbacks++ = NULL;
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", startIntrDMA);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", trapIntrDMA);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libetc", setIntrDMA);

static void memclrIntrDMA(void(**callbacks)(void), unsigned int numCallbacks) {
    while (numCallbacks--) {
        *callbacks++ = NULL;
    }
}

long SetVideoMode(long mode) {
    long prev;

    prev = g_VideoMode;
    g_VideoMode = mode;
    return prev;
}

long GetVideoMode(void) { 
    return g_VideoMode; 
}
