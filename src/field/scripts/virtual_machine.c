#include "common.h"
#include "field/main.h"
#include "field/actor.h"
#include "system/memory.h"
#include "psyq/libgpu.h"

extern Actor* g_FieldScriptVMCurActor;
extern void* g_FieldScriptVMCurScriptData;
extern int g_FieldScriptMaxInstructionCount;
extern void* g_FieldScriptMemory;
extern ScriptsFile* g_FieldCurScriptFile;

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

