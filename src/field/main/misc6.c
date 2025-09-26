#include "common.h"
#include "field/script_vm.h"
#include "field/actor.h"
#include "field/text_box.h"

extern void func_80076AC0(s32, s32, void*, s32, s32, s32, s32);
extern void func_800A0C94();
extern s32 D_800AFD1C; // Current actor index

void FieldScriptVMHandlerDisableDialogActivation(void) {
    g_FieldScriptVMCurActor->scriptFlags |= SCRIPT_DIALOG_ACTIVATION_DISABLED;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

void func_8009DA44(void) {
    g_FieldScriptVMCurActor->scriptFlags &= ~0x20000;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

void func_8009DA70(void) {
    g_FieldScriptVMCurActor->scriptFlags |= 0x800000;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

void func_8009DA98(void) {
    g_FieldScriptVMCurActor->scriptFlags &= ~0x800000;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

void func_8009DAC4(void) {
    int nTextBoxIndex;
    ActorData* pActor;
    FieldActor* pFieldActor;

    if (FieldScriptVMGetActorIndex(1) != 0xFF) {
        pActor = g_FieldActors[FieldScriptVMGetActorIndex(1)].pActorData;
        pActor->scriptFlags |= 0x1;
        pActor->flags |= 0x100000;
        pFieldActor = &g_FieldActors[FieldScriptVMGetActorIndex(1)];
        pFieldActor->status |= ACTOR_STATUS_INVISIBLE;
        if (func_8009CD18(&nTextBoxIndex) == 0) {
            g_FieldTextBoxes[nTextBoxIndex].status = 0;
        }
    }
    g_FieldScriptVMCurActor->scriptInstructionPointer += 2;
}

void FieldScriptVMHandlerEnableActorVM(void) {
    ActorData* pActor;

    if (FieldScriptVMGetActorIndex(1) != 0xFF) {
        pActor = g_FieldActors[FieldScriptVMGetActorIndex(1)].pActorData;
        pActor->scriptFlags &= ~SCRIPT_VM_DISABLED;
    }
    g_FieldScriptVMCurActor->scriptInstructionPointer += 2;
}

void func_8009DC4C(void) {
    ActorData* pActor;
    int nTextBoxIndex;
    short nNewRotation;

    if (FieldScriptVMGetActorIndex(1) != 0xFF) {
        pActor = g_FieldActors[FieldScriptVMGetActorIndex(1)].pActorData;
        pActor->moveModified.vx = 0;
        pActor->moveModified.vy = 0;
        pActor->moveModified.vz = 0;
        pActor->move.vx = 0;
        pActor->move.vy = 0;
        pActor->move.vz = 0;
        pActor->scriptFlags |= 1;
        nNewRotation = pActor->rotationX | 0x8000;
        pActor->rotationY = nNewRotation;
        pActor->rotationX = nNewRotation;
        if (func_8009CD18(&nTextBoxIndex) == 0) {
            g_FieldTextBoxes[nTextBoxIndex].status = 0;
        }
    }
    g_FieldScriptVMCurActor->scriptInstructionPointer += 2;
}

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

void FieldScriptVMHandlerShowActorById(void) {
    FieldActor* pFieldActor;

    if (FieldScriptVMGetActorIndex(1) != 0xFF) {
        pFieldActor = &g_FieldActors[FieldScriptVMGetActorIndex(1)];
        if (!(pFieldActor->pActorData->flags & 0x100000)) {
            pFieldActor->status &= ~ACTOR_STATUS_INVISIBLE;
            pFieldActor->pActorData->flags &= ~0x2000000;
        }
    }
    g_FieldScriptVMCurActor->scriptInstructionPointer += 2;
}

void FieldScriptVMHandlerHideActorById(void) {
    FieldActor* pFieldActor;

    if (FieldScriptVMGetActorIndex(1) != 0xFF) {
        pFieldActor = &g_FieldActors[FieldScriptVMGetActorIndex(1)];
        pFieldActor->status |= ACTOR_STATUS_INVISIBLE;
    }
    g_FieldScriptVMCurActor->scriptInstructionPointer += 2;
}

void FieldScriptVMHandlerShowActor(void) {
    FieldActor* pActor = &g_FieldActors[D_800AFD1C];
    pActor->status &= ~ACTOR_STATUS_INVISIBLE;
    g_FieldScriptVMCurActor->curAnimationId = 0xFF;
    g_FieldScriptVMCurActor->flags &= 0xFDFFFFFF;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

void func_8009DF78(void) {
    FieldActor* pFieldActor;

    if (FieldScriptVMGetActorIndex(1) != 0xFF) {
        pFieldActor = &g_FieldActors[FieldScriptVMGetActorIndex(1)];
        pFieldActor->pActorData->flags |= 0x02000000;
        pFieldActor->pActorData->flags |= 0x800;
    }
    g_FieldScriptVMCurActor->scriptInstructionPointer += 2;
}

void func_8009E014(void) {
    g_FieldScriptVMCurActor->flags |= 0x02000800;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

void FieldScriptVMHandlerHideActor(void) {
    FieldActor* pActor = &g_FieldActors[D_800AFD1C];
    pActor->status |=  ACTOR_STATUS_INVISIBLE;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

// Update sprite movement or animation speed?
void func_8009E094(void) {
    unsigned short nMoveSpeed;

    nMoveSpeed = FieldScriptVMGetArgument(1);
    g_FieldScriptVMCurActor->moveSpeed = nMoveSpeed;
    func_80021BCC(g_FieldActors[D_800AFD1C].pSpriteData, nMoveSpeed);
    g_FieldScriptVMCurActor->scriptInstructionPointer += 3;
}

void func_8009E10C(void) {
    int nValue;
    int nUnkFlags;

    nValue = FieldScriptVMGetArgument(1);
    nUnkFlags = (nValue & 0x1) << 7; // if (nValue & 0x1) nUnkFlags |= 0x80;
    if (nValue & 0x4) nUnkFlags |= 0x20;
    if (nValue & 0x8) nUnkFlags |= 0x10;
    if (nValue & 0x10) nUnkFlags |= 8;
    if (nValue & 0x20) nUnkFlags |= 4;
    if (nValue & 0x40) nUnkFlags |= 0x08000000;
    
    g_FieldScriptVMCurActor->scriptFlags &= 0xF7FFFF43;
    g_FieldScriptVMCurActor->scriptFlags |= nUnkFlags;
    g_FieldScriptVMCurActor->scriptInstructionPointer += 3;
}

void func_8009E1A0(void) {
    u32 nValue;

    nValue = SCRIPT_READ_U8_REL(1) & 7;
    g_FieldScriptVMCurActor->flags &= ~7;
    g_FieldScriptVMCurActor->flags |= nValue;
    
    nValue = (SCRIPT_READ_U8_REL(1) >> 1) & 0x38;
    g_FieldScriptVMCurActor->flags &= ~0x38;
    g_FieldScriptVMCurActor->flags |= nValue;
    
    g_FieldScriptVMCurActor->scriptInstructionPointer += 2;
}

void func_8009E208(void) {
    g_FieldScriptVMCurActor->unkEC = 0;
    g_FieldScriptVMCurActor->scriptFlags = (g_FieldScriptVMCurActor->scriptFlags & 0xFFFBFFFF) | 0x400000;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
    g_FieldScriptVMCurActor->curYPos = g_FieldScriptVMCurActor->position.vy >> 0x10;
}

void func_8009E248(void) {
    func_8009E574(
        FieldScriptVMGetInstructionArgumentS16(1),
        FieldScriptVMGetInstructionArgumentS16(3)
    );
    func_8009E810(FieldScriptVMGetInstructionArgumentS16(5));
    g_FieldScriptVMCurActor->scriptFlags |= 0x40000;
    g_FieldScriptVMCurActor->scriptInstructionPointer += 7;
}

void func_8009E2C8(void) {
    func_8009E810(func_8009CF78(1, SCRIPT_READ_U8_REL(3)));
    g_FieldScriptVMCurActor->scriptFlags |= 0x40000;
    g_FieldScriptVMCurActor->scriptInstructionPointer += 4;
}

// Read short from bytecode, absolute offset
short func_8009E330(int offset) {
    return SCRIPT_READ_U8(offset) + (SCRIPT_READ_U8(offset + 1) << 8);
}

void func_8009E35C(void) {
    g_FieldScriptVMCurActor->walkmeshId = SCRIPT_READ_U8_REL(5);
    func_8009E574(
        func_8009CF78(1, SCRIPT_READ_U8_REL(6)), 
        func_8009CFBC(3, SCRIPT_READ_U8_REL(6))
    );
    g_FieldScriptVMCurActor->flags &= ~0x200000;
    g_FieldScriptVMCurActor->scriptFlags &= ~0x10000;
    g_FieldScriptVMCurActor->scriptInstructionPointer += 7;
}

void func_8009E428(void) {
    ActorData* pActor;

    g_FieldScriptVMCurActor->walkmeshId = SCRIPT_READ_U8_REL(1);
    pActor = g_FieldActors[D_800AFD1C].pActorData;
    func_8009E574(pActor->position.vx >> 0x10, pActor->position.vz >> 0x10);
    g_FieldScriptVMCurActor->scriptInstructionPointer += 2;
}

void func_8009E4BC(void) {
    func_8009E574(
        func_8009CF78(1, SCRIPT_READ_U8_REL(5)), 
        func_8009CFBC(3, SCRIPT_READ_U8_REL(5))
    );
    g_FieldScriptVMCurActor->flags &= ~0x200000;
    g_FieldScriptVMCurActor->scriptFlags &= ~0x10000;
    g_FieldScriptVMCurActor->scriptInstructionPointer += 6;
}


INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E574);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_8009E810);

void func_8009E83C(void) {
    unsigned char width;
    unsigned char zWidth;
    unsigned char height;
    unsigned char solidRange;

    width = SCRIPT_READ_U8_REL(1);
    if (width) {
        g_FieldScriptVMCurActor->width = width * 2;
    }
    
    zWidth = SCRIPT_READ_U8_REL(2);
    if (zWidth) {
        g_FieldScriptVMCurActor->zWidth = zWidth * 2;
    }
    
    height = SCRIPT_READ_U8_REL(3);
    if (height) {
        g_FieldScriptVMCurActor->height = height * 2;
    }
    
    solidRange = SCRIPT_READ_U8_REL(4);
    if (solidRange) {
        g_FieldScriptVMCurActor->solidRange = SCRIPT_READ_U8_REL(4) * 2;
    }
    
    g_FieldScriptVMCurActor->scriptInstructionPointer += 5;
}

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

// FieldResetActorPosition, set translation and sprite position of actor based on current actor data position
INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0C94);

void func_800A0D3C(void) {
    func_80076AC0(D_800AFD1C, 0, (void*)((*(s32*)(g_FieldSpriteData + 4)) + (s32)g_FieldSpriteData), 0, 0, 0x80, 1);
    func_800A0C94();
    g_FieldScriptVMCurActor->scriptFlags |= 0x100;
    g_FieldScriptVMCurActor->flags |= 0x800;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0DC0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0DFC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0E54);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0EB0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0EE8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A0FD8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A1364);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A14F0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc6", func_800A1624);
