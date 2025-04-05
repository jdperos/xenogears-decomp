#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#define MAX_INTERRUPT_CALLBACKS 8

typedef void (*VoidCallback_t)(void);
typedef void (*SetVsyncIntrCallback_t)(u32, void(*)(void));

typedef struct {
    const char* version;
    void(*setDMAIntrCallback)(u32 index, VoidCallback_t callback);
    void(*setIntrHandler)(u32 irqLine, VoidCallback_t callback);
    void(*intrResetHandler)(void);
    void(*intrStopHandler)(void);
    void(*setVsyncIntrCallback)(u32 index, VoidCallback_t callback);
    void(*intrUnkHandler2)(void);
} InterruptControl_t;

extern InterruptControl_t g_InterruptControl;

extern VoidCallback_t g_VsyncInterruptCallbacks[MAX_INTERRUPT_CALLBACKS];

extern volatile unsigned int g_VsyncInterruptCount;
extern volatile u32* g_pTMR_HRETRACE_MODE;

#endif