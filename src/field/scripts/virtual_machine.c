#include "common.h"
#include "field/main.h"
#include "field/actor.h"
#include "field/script_vm.h"
#include "system/memory.h"
#include "psyq/libgpu.h"

extern s32 D_800AFFEC;
extern s32 D_800AFD1C; // Current actor index
extern s32 D_800B00C0;

extern void func_800379C8(char*, ...);
extern char D_8006FD44; // "STACKERR ACT=%d\n"

void func_800A1730(void) {
    u_int nFlags;
    u_int nNewFlags;

    nFlags = g_FieldScriptVMCurActor->flags12C;
    if ((nFlags & 0x1C0) != 0x100) {
        g_FieldScriptVMCurActor->scriptPointersStack[(nFlags >> 6) & 0x7] = g_FieldScriptVMCurActor->scriptInstructionPointer + 5;
        g_FieldScriptVMCurActor->scriptInstructionPointer = FieldScriptVMGetInstructionArgument(1);
        nNewFlags = g_FieldScriptVMCurActor->flags12C;
        g_FieldScriptVMCurActor->flags12C = (nNewFlags & ~0x1C0) | (((((nNewFlags >> 6) & 7) + 1) & 7) << 6);
        return;
    }

    // Error
    if (g_FieldSystemMode == 0) {
        func_800379C8(&D_8006FD44, D_800AFD1C, nFlags);
    }
    D_800B00C0 = 1;
}

void func_800A17F4(void) {
    u_int nFlags;
    u_int nCurFlags;

    nFlags = g_FieldScriptVMCurActor->flags12C;
    if ((nFlags & 0x1C0) != 0x100) {
        g_FieldScriptVMCurActor->scriptPointersStack[(nFlags >> 6) & 0x7] = g_FieldScriptVMCurActor->scriptInstructionPointer + 3;
        g_FieldScriptVMCurActor->scriptInstructionPointer = FieldScriptVMGetInstructionArgument(1);
        nCurFlags = g_FieldScriptVMCurActor->flags12C;
        g_FieldScriptVMCurActor->flags12C =  ((nCurFlags & ~0x1C0) | (((((nCurFlags >> 6) & 7) + 1) & 7) << 6));
        return;
    }
    
    // Error
    if (g_FieldSystemMode == 0) {
        func_800379C8(&D_8006FD44, D_800AFD1C);
    }
    
    D_800B00C0 = 1;
}

void func_800A18B8(void) {
    u_int nNewFlags;
    u_int nFlags;

    nFlags = g_FieldScriptVMCurActor->flags12C;

    // Error, invalid stack value
    if (!(nFlags & 0x1C0)) {
        if (g_FieldSystemMode == 0) {
            func_800379C8(&D_8006FD44, D_800AFD1C);
        }
        g_FieldScriptVMCurActor->eventSlots[ g_FieldScriptVMCurActor->curEventSlotId].flags |= 0x3C0000;
        g_FieldScriptVMCurActor->eventSlots[g_FieldScriptVMCurActor->curEventSlotId].eventId = 0xFF;
        D_800AFFEC = 1;
        D_800B00C0 = 1;
        return;
    }
    
    nNewFlags = (nFlags & ~0x1C0) | (((((nFlags >> 6) & 7) - 1) & 7) << 6);
    g_FieldScriptVMCurActor->flags12C = nNewFlags;
    g_FieldScriptVMCurActor->scriptInstructionPointer = g_FieldScriptVMCurActor->scriptPointersStack[(nNewFlags >> 6) & 0x7];
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A19B0);


void func_800A1A8C(void) {
    int i;

    for (i = 0; i < 8; i++) {
        if ( ((g_FieldScriptVMCurActor->eventSlots[i].flags >> 0x12) & 0xF) == 7) {
            g_FieldScriptVMCurActor->eventSlots[i].reqEvent = func_800A3090(D_800AFD1C, 1);
        }
    }

    g_FieldScriptVMCurActor->eventSlots[g_FieldScriptVMCurActor->curEventSlotId].flags |=  0x3C0000;
    g_FieldScriptVMCurActor->eventSlots[g_FieldScriptVMCurActor->curEventSlotId].eventId = 0xFF;
    D_800B00C0 = 1;
}

void func_800A1B70(void) {
    g_FieldScriptVMCurActor->eventSlots[g_FieldScriptVMCurActor->curEventSlotId].flags |= 0x3C0000;
    g_FieldScriptVMCurActor->eventSlots[g_FieldScriptVMCurActor->curEventSlotId].eventId = 0xFF;
    D_800AFFEC = 1;
    D_800B00C0 = 1;
}

void FieldScriptVMHandlerConditionalJmp(void) {
    int nValue2;
    int nValue1;
    int nArgumentsType;
    int nCondCheck;
    int nCondResult;
    int nConditionType;

    nValue2 = 0;
    nValue1 = 0;
    nArgumentsType = ((u_char*)g_FieldScriptVMCurScriptData)[g_FieldScriptVMCurActor->scriptInstructionPointer + 5] & 0xF0;
    switch (nArgumentsType) {
    case 0x0:
        nValue1 = FieldScriptVMReadArgumentFromMemory(FieldScriptVMGetInstructionArgument(1) & 0xFFFF);
        nValue2 = FieldScriptVMReadArgumentFromMemory(FieldScriptVMGetInstructionArgument(3) & 0xFFFF);
        if (FieldScriptVMGetVariableSign(FieldScriptVMGetInstructionArgument(1) & 0xFFFF) != FIELD_SCRIPT_VM_VAR_SIGNED) {
            nValue2 = (u_short)nValue2;
        } else {
            nValue2 = (short)nValue2;
        }
        break;
    case 0x40:
        nValue1 = FieldScriptVMReadArgumentFromMemory(FieldScriptVMGetInstructionArgument(1) & 0xFFFF);
        nValue2 = func_800ACD7C(3);
        if (FieldScriptVMGetVariableSign(FieldScriptVMGetInstructionArgument(1) & 0xFFFF) != FIELD_SCRIPT_VM_VAR_SIGNED) {
            nValue2 = (u_short)nValue2;
        }
        break;
    case 0x80:
        nValue1 = func_800ACD7C(1);
        nValue2 = FieldScriptVMReadArgumentFromMemory(FieldScriptVMGetInstructionArgument(3) & 0xFFFF);
        if (FieldScriptVMGetVariableSign(FieldScriptVMGetInstructionArgument(3) & 0xFFFF) != FIELD_SCRIPT_VM_VAR_SIGNED) {
            nValue1 = (u_short)nValue1;
        }
        break;
    case 0xC0:
        nValue1 = func_800ACD7C(1);
        nValue2 = func_800ACD7C(3);
        break;
    }
    
    nCondResult = 0;
    nConditionType = ((u_char*)g_FieldScriptVMCurScriptData)[g_FieldScriptVMCurActor->scriptInstructionPointer + 5] & 0xF;
    switch (nConditionType) {
        case FIELD_SCRIPT_VM_COND_EQUAL:
            if (nValue1 == nValue2) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_NOT_EQUAL2:
            if (nValue1 != nValue2) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_LT:
            nCondCheck = nValue2 < nValue1;
            if (nCondCheck) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_LT2:
            nCondCheck = nValue1 < nValue2;
            if (nCondCheck) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_GTE:
            if (nValue1 >= nValue2) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_GTE2:
            if (nValue2 >= nValue1) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_AND:
            nCondCheck = nValue1 & nValue2;
            if (nCondCheck) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_NOT_EQUAL:
            if (nValue1 != nValue2) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_OR:
            nCondCheck = nValue1 | nValue2;
            if (nCondCheck) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_AND2:
            nCondCheck = nValue1 & nValue2;
            if (nCondCheck) nCondResult++;
            break;
        case FIELD_SCRIPT_VM_COND_NAND:
            nCondCheck = ~nValue1 & nValue2;
            if (nCondCheck) nCondResult++;
            break;
    }
    
    if (nCondResult == 1) {
        g_FieldScriptVMCurActor->scriptInstructionPointer = g_FieldScriptVMCurActor->scriptInstructionPointer + 8;
    } else {
        g_FieldScriptVMCurActor->scriptInstructionPointer = FieldScriptVMGetInstructionArgument(6);
    }
}

void FieldScriptVMHandlerJmp(void) {
    g_FieldScriptVMCurActor->scriptInstructionPointer = FieldScriptVMGetInstructionArgument(1);
}

void func_800A1E9C(void) {
    g_FieldScriptMaxInstructionCount += 0x20;
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", FieldScriptVMRun);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A2030);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A22AC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A2488);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A24C4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A2714);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A28D4);

void FieldScriptVMHandlerNop(void) {
    g_FieldScriptVMCurActor->scriptInstructionPointer++;
}

int FieldScriptVMGetVariableSign(int index) {
    return -((g_FieldCurScriptFile->signBits[index >> 6] & (1 << ((index >> 1) & 0x1F))) != 0);
}

int FieldScriptVMReadArgumentFromMemory(int index) {
    if (!(g_FieldCurScriptFile->signBits[index >> 6] & (1 << ((index >> 1) & 0x1F)))) {
        return ((short*)&g_FieldScriptMemory)[index >> 1];
    } else {
        return ((u_short*)&g_FieldScriptMemory)[index >> 1];
    }
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", FieldScriptMemoryWriteU16);

u_short func_800A3090(int scriptIndex, int dataOffset) {
    int nOffset;
    u_short* pScriptData;

    pScriptData = &g_FieldCurScriptFile->metadata;
    nOffset = (scriptIndex * (SCRIPT_SIZE / sizeof(u_short)) + dataOffset);
    return *(pScriptData + nOffset);
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A30B4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A30FC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A31E8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A3474);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A3C8C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A3F4C);

void func_800A4748(void) {
    func_800A476C(0x2c0,0x100);
}

void func_800A476C(int x, int y) {
    RECT rect;

    rect.w = 0x140;
    rect.y = 0;
    rect.x = 0;
    rect.h = 0xe0;
    SetGeomScreen(0x200);
    MoveImage(&rect, x, y);
    FieldRenderSync();
}

