#ifndef _XENO_FIELD_ACTOR_H
#define _XENO_FIELD_ACTOR_H

#include "psyq/libgte.h"

#define ACTOR_STATUS_INVISIBLE 0x20

#define SCRIPT_VM_DISABLED 0x1
#define SCRIPT_DIALOG_ACTIVATION_DISABLED 0x20000

#define MASK_8DIR_MOVEMENT_NUM_DIRECTIONS 0x7

typedef struct {
    u_short reqEvent;
    u_char waitTimer;
    u_char eventId;
    u_int flags;
} ActorEventSlot;

typedef struct {
    short x, y, z;
} SVEC;

typedef struct {
    /* 0x0  */ u_int scriptFlags;
    /* 0x4  */ u_int flags; // ?
    /* 0x6  */ u_short walkmesh0TriId;
    /* 0x8  */ u_short walkmesh1TriId;
    /* 0xA  */ u_short walkmesh2TriId;
    /* 0xC  */ u_short walkmesh3TriId;
    u_int walkmeshId;
    u_int curWalkmeshTriMaterial;
    u_short width; //xWidth
    u_short height;
    u_short zWidth;
    u_short solidRange;
    VECTOR position;
    VECTOR moveModified;
    VECTOR move;
    VECTOR curTriNormal;
    SVECTOR unk60;
    SVEC prevPosition;
    /* 0x6E  */ u_short unk6E;
    /* 0x70  */ u_short unk70;
    /* 0x72  */ u_short curYPos;
    /* 0x74  */ u_char canInteract;
    /* 0x75  */ u_char parentActorId;
    /* 0x76  */ u_short moveSpeed;
    /* 0x78  */ short scriptPointersStack[4];
    u_char faceId;
    u_char unk81;
    u_char dialogWidth;
    u_char dialogHeight;
    u_int dialogFlags;
    u_short dialogPixelWidth;
    u_short dialogPixelHeight;
    ActorEventSlot eventSlots[8];
    /* 0xCC */ u_short scriptInstructionPointer;
    /* 0xCE  */ u_char curEventSlotId;
    /* 0xCF  */ u_char unkCF;
    /* 0xD0  */ VECTOR unkD0;
    /* 0xE0  */ u_short unkE0;
    /* 0xE2  */ u_char curDoorStep;
    /* 0xE3  */ u_char unkE3; // timer?
    /* 0xE4  */ u_short characterId;
    /* 0xE6  */ u_short defaultAnimationId;
    /* 0xE8  */ u_short curAnimationId;
    /* 0xEA  */ u_short unkAnimationId;
    /* 0xEC  */ int unkEC;
    /* 0xF0  */ int unkF0;
    /* 0xF4  */ u_short scaleX;
    /* 0xF6  */ u_short scaleY;
    /* 0xF8  */ u_short scaleZ;
    /* 0xFA  */ short unkFA;
    /* 0xFC  */ u_char unkFC;
    /* 0xFD  */ u_char unkFD;
    /* 0xFE  */ u_char unkFE;
    /* 0xFF  */ u_char unkFF;
    /* 0x100 */ u_char unk100;
    /* 0x101 */ u_char unk101;
    /* 0x102 */ short unk102;
    /* 0x104 */ short rotationX; // 0xFFF mask: rotation
    /* 0x106 */ short rotationY;
    /* 0x108 */ short rotationZ;
    /* 0x10A */ short unk10A;
    /* 0x10C */ u_char unk10C;
    /* 0x10D */ u_char unk10D;
    /* 0x110 */ void* unk110;
    /* 0x114 */ void* unk114;
    /* 0x118 */ void* unk118;
    /* 0x11C */ short unk11C;
    /* 0x11E */ short unk11E;
    /* 0x120 */ void* unk120;
    /* 0x124 */ short unk124;
    /* 0x126 */ u_char unk126;
    /* 0x127 */ u_char spriteId;
    /* 0x128 */ u_int modelAnimation;


    // 0x1C0 => Stack index, bit 0x100 is not used since there's only four elements in the stack
    u_int flags12C;


    int flags130;
    int flags134;
} ActorData;

typedef struct {
    void* pModelData; // 0x24 size, model related data
    void* pSpriteData; // 0x164 size
    void* pShadow; // 0x70 size
    MATRIX transformMatrix;
    MATRIX childMatrix;
    ActorData* pActorData;
    SVEC rotation;
    short flags;
    short status;
} FieldActor;

typedef struct {
    short L11;
    short L12;
    short L13;
    short LR1;
    short LR2;
    short LR3;
    short L21;
    short L22;
    short L23;
    short LG1;
    short LG2;
    short LG3;
    short L31;
    short L32;
    short L33;
    short LB1;
    short LB2;
    short LB3;
    short RBK;
    short GBK;
    short BBK;
} FieldLightFileData;

typedef struct {
    short flags;
    short rotationX;
    short rotationY;
    short rotationZ;
    short positionX;
    short positionY;
    short positionZ;
    short modelID;
} FieldActorFileData;

typedef struct {

    // Decompressed sizes
    /* 0x10C */ u_int timPackageSize;
    /* 0x110 */ u_int walkmeshSize;
    /* 0x114 */ u_int modelDataSize;
    /* 0x118 */ u_int spriteDataSize;
    /* 0x11C */ u_int clutDataSize;
    /* 0x120 */ u_int scriptsSize;
    /* 0x124 */ u_int unk3Size;
    /* 0x128 */ u_int dialogsSize;
    /* 0x12C */ u_int triggersSize;

    // Offsets to compressed data
    /* 0x130 */ u_int timPackageOffset;
    /* 0x134 */ u_int walkmeshDataOffset;
    /* 0x138 */ u_int modelDataOffset;
    /* 0x13C */ u_int spriteDataOffset;
    /* 0x140 */ u_int clutDataOffset;
    /* 0x144 */ u_int scriptsOffset;
    /* 0x148 */ u_int pUnk3Data;
    /* 0x14C */ u_int dialogsOffset;
    /* 0x150 */ u_int triggersOffset;
    /* 0x154 */ FieldLightFileData lightData;
    /* 0x18C */ u_int numEntitites;
    /* 0x190 */ void* actorData; // Start of entity data
} ActorFile;


#define SCRIPT_SIZE 0x40

typedef struct {
    u_int signBits[0x20]; // Sign bits for variables
    /* 0x80 */ u_int numScripts;
    /* 0x84 */ void* metadata; // numScripts * 0x40 size of script metadata
    // script instructions
} ScriptsFile;

extern int g_FieldNumActors;
extern FieldActor* g_FieldActors;

extern ActorData* g_FieldScriptVMCurActor;
extern void* g_FieldScriptVMCurScriptData;
extern int g_FieldScriptMaxInstructionCount;
extern void* g_FieldScriptMemory;
extern ScriptsFile* g_FieldCurScriptFile;
extern int D_800B00C0; // Stop script VM exection?

extern void* g_FieldSpriteData;

#define SCRIPT_READ_U8(idx) *(u8*)&g_FieldScriptVMCurScriptData[idx]
#define SCRIPT_READ_U8_REL(idx) *(u8*)&g_FieldScriptVMCurScriptData[g_FieldScriptVMCurActor->scriptInstructionPointer + idx]

#endif