#include "common.h"

// Sprite / Animation functions

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80022B2C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80022CAC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80022CDC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80022D44);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80022DF4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80022E8C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80022EB8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80022FC4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_8002303C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800230A8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023124);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023170);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800231E0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800231F8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023210);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023290);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023340);

// Allocate struct stuff
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800233A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023440);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023468);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800234AC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023538);

// SpriteData stuff
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023804);



INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_8002393C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023950);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023958);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800239A0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800239F4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023A48);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023B84);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80023FD8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80024294);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800242F4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_8002435C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80024524);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800245D8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80024730);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800248D4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80024F20);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80024F64);
/*
extern void* D_800594B8;

s32 D_800592FC;
s32 D_80059300;
s32 D_80059304;
void* D_800594B4;
s32 D_800594C4;

void func_80024F64(int numEntries, unsigned int allocFlag) {
    void* pBuffer;

    D_800592FC = numEntries;
    pBuffer = HeapAlloc(numEntries * 2, allocFlag);
    D_800594B4 = pBuffer;
    D_800594B8 = pBuffer + numEntries;
    D_80059304 = 0;
    D_80059300 = 0;
    D_800594C4 = 0;
    func_8001D298();
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80024FB8);
/*
void func_80024FB8(void) {
    HeapFree(D_800594B4);
    func_8001D2A4();
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80024FE4);
// Set OT for this part of the graphics system
/*void func_80024FE4(u_long* ot) {
    D_8005956C = ot;
}
*/








// Maybe start of a TU
// ===========================================================


// Set D_8004FBB8 matrix
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80024FF4);
/*
Matches on GCC 2.8.0.
Co-Authored-By: dezgeg <dezgeg@users.noreply.github.com>

extern MATRIX D_8004FBB8;
void func_80024FF4(MATRIX* matrix) {
    D_8004FBB8 = *matrix;
}
*/











// Maybe start of a TU
// ===========================================================

// Loops over the linked list given by D_800594C4[D_800592F8].
// LoadImage on entries with an address, ClearImage otherwise.
// Set the list to NULL afterwards.
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025044);

// This function is in another TU than func_80024F64 due to GP Rel variables
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800250E0);


INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025180);

// Add Image to current D_800594C4[D_800592F8] linked list
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800251C8);
/*
Matches, but uses variable in COMMON so can't compile in yet.

struct ImageEntry {
    RECT rect;
    u_long* addr;
    struct ImageEntry* pNext;
};

typedef struct ImageEntry ImageEntry;

int D_800592F8; // Cur index?
void* D_80059534; // End address?
extern ImageEntry* D_800594C4[];
ImageEntry* D_80059580; // List head

void func_800251C8(u_long* addr, int x, int y, int width, int height) {
    ImageEntry* pCurrent;
    ImageEntry* pNext;

    pCurrent = D_80059580;
    pNext = pCurrent + 1;
    if (pNext < D_80059534) {
        pCurrent->rect.h = height;
        pCurrent->rect.x = x;
        pCurrent->rect.y = y;
        pCurrent->rect.w = width;
        pCurrent->addr = addr;
        
        D_80059580 = pNext;
        pCurrent->pNext = D_800594C4[D_800592F8];
        D_800594C4[D_800592F8] = pCurrent;
    }
}
*/

// Sets pStruct->unk8 callback
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025224);
/*
CALLBACK_TABLE
8004fd40 func_80025258
8004fd44 func_80025710 => Dummy function
8004fd48 func_80025718
8004fd4c NULL
8004fd50 NULL
8004fd54 func_80025258
8004fd58 func_80025258
8004fd5c func_80025718
8004fd60 func_8002541c
8004fd64 func_80025544
8004fd68 NULL
8004fd6c NULL
8004fd70 NULL
8004fd74 NULL
8004fd78 func_80025258
8004fd7c func_800257f0

void func_80025224(void* pStruct,int handlerIndex) {
    func_8001CD64(pStruct, &CALLBACK_TABLE[handlerIndex]);
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025258);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_8002541C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025544);

void func_80025710(void) {}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025718);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800257F0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025A88);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025C04);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025D4C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80025FA8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80026338);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800263E4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_8002675C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80026A0C);

void func_80026B9C(void) {
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80026BA4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80026DCC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80026F44);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80026FE8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_8002709C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800273C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_800278F8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80027D40);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80027D64);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_80027EAC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_8002800C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/temp1", func_8002804C);
