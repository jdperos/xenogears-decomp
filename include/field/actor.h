#ifndef _XENO_FIELD_ACTOR_H
#define _XENO_FIELD_ACTOR_H

#include "psyq/libgte.h"

typedef struct {
    u_short reqEvent;
    u_char shouldWait;
    u_char eventId;
    u_int flags;
} ActorEventSlot;

typedef struct {
    short x, y, z;
} SVEC;

typedef struct {
    u_int scriptFlags;
    u_int flags; // ?
    u_short walkmesh0TriId;
    u_short walkmesh1TriId;
    u_short walkmesh2TriId;
    u_short walkmesh3TriId;
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
    u_short unk6E;
    u_short unk70;
    u_short curYPos;
    u_char canInteract;
    u_char parentActorId;
    u_short moveSpeed;
    u_int scriptPointersStack[2];
    u_char faceId;
    u_char unk81;
    u_char dialogWidth;
    u_char dialogHeight;
    u_int dialogFlags;
    u_short dialogPixelWidth;
    u_short dialogPixelHeight;
    ActorEventSlot eventSlots[8];
    u_short scriptInstructionPointer;
    u_char curEventSlotId;
    u_char unkCF;
    VECTOR unkD0;
    u_short unkE0;
    u_char curDoorStep;
    u_char unkE3; // timer?
    u_short characterId;
    u_short defaultAnimationId;
    u_short curAnimationId;
    u_short unkAnimationId;
    u_short unkEC;
    u_short unkF0;
    u_short scaleX;
    u_short scaleY;
    u_short scaleZ;
} Actor;


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
    /* 0x13C */ u_int pCompressedSpriteData;
    /* 0x140 */ u_int clutDataOffset;
    /* 0x144 */ u_int scriptsOffset;
    /* 0x148 */ u_int pUnk3Data;
    /* 0x14C */ u_int dialogsOffset;
    /* 0x150 */ u_int triggersOffset;
    /* 0x154 */ FieldLightFileData lightData;
    /* 0x18C */ u_int numEntitites;
    /* 0x190 */ void* actorData; // Start of entitiy data
} ActorFile;


#define SCRIPT_SIZE 0x40

typedef struct {
    u_int signBits[0x20]; // Sign bits for variables
    /* 0x80 */ u_int numScripts;
    /* 0x84 */ void* metadata; // numScripts * 0x40 size of script metadata
    // script instructions
} ScriptsFile;

#endif