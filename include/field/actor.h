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

#endif