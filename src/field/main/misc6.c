#include "common.h"
#include "field/script_vm.h"
#include "field/actor.h"

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DA1C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DA44);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DA70);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DA98);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DAC4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DBC8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DC4C);

void FieldScriptVMHandlerSleep(void) {
    u_char nEventSlotId;

    nEventSlotId = g_FieldScriptVMCurActor->curEventSlotId;

    // Initialize the value with the argument as the time to sleep, else count down
    if (g_FieldScriptVMCurActor->eventSlots[nEventSlotId].waitTimer == 0) {
        g_FieldScriptVMCurActor->eventSlots[g_FieldScriptVMCurActor->curEventSlotId].waitTimer = FieldScriptVMGetArgument(1);
    } else {
        g_FieldScriptVMCurActor->eventSlots[nEventSlotId].waitTimer--;
    }
    
    // When the timer has reached 0, we move to the next instruction
    if (g_FieldScriptVMCurActor->eventSlots[g_FieldScriptVMCurActor->curEventSlotId].waitTimer == 0) {
        g_FieldScriptVMCurActor->scriptInstructionPointer += 3;
    }

    D_800B00C0 = 1;
}


INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DDEC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DE94);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DF10);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009DF78);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E014);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E040);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E094);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E10C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E1A0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E208);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E248);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E2C8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E330);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E35C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E428);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E4BC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E574);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E810);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E83C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E91C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009EB48);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009EB78);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009ED68);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009F0A0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009F424);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009F4CC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009F5A8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009F5F4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FA00);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FA54);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FB98);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FC10);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FC48);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FCAC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FD10);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FDD4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FE4C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009FEE4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0158);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0228);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0524);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A06E8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A08B8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0C4C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0C94);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0D3C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0DC0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0DFC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0E54);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0EB0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0EE8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0FD8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A1364);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A14F0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A1624);
