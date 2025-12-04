#ifndef _XENO_GAME_H
#define _XENO_GAME_H

#define MAX_GAME_CHARACTERS 11
#define MAX_GAME_GEARS 20
#define MAX_PARTY_MEMBERS 3

typedef struct {
    /* 0x0  */ u8 field_0x0[0x4C];
    /* 0x4C */ unsigned short hp;
    /* 0x4E */ unsigned short maxHp;
    /* 0x50 */ unsigned short mp;
    /* 0x52 */ unsigned short maxMp;
    /* 0x54*/  u32 field_0x54;
    /* 0x58 */ u8 attack;
    /* 0x59 */ u8 defense;
    /* 0x5A */ u8 agility;
    /* 0x5B */ u8 ether;
    /* 0x5C */ u8 etherDefence;
    /* 0x5D */ u8 field_0x5D;
    /* 0x5E */ u8 hitPercentage;
    /* 0x5F */ u8 evadePercentage;
    /* 0x60 */ u16 field_0x60;
    /* 0x61 */ u8 level;
    /* 0x62 */ u8 field_0x62[0x3f];
} GameCharacter; // size: 0xA4

typedef struct {
    /* 0x0  */ u8 field_0x0[0x10];
    /* 0x10 */ s16 ether;
    /* 0x12 */ s16 maxEther;
    /* 0x14 */ u8 field_0x14[0x24];
    /* 0x38 */ s32 fuel;
    /* 0x3C */ s32 maxFuel;
    /* 0x40 */ u8 field_0x40[0x20];
    /* 0x60 */ unsigned int hp;
    /* 0x64 */ unsigned int maxHp;
    /* 0x68 */ u8 field_0x68[0x3C];
} GameGear; // size: 0xA4

typedef struct {
    /* 0x0    */ u8 field_0x0[0x26C];
    /* 0x26C  */ GameCharacter characters[MAX_GAME_CHARACTERS];
    /* 0x978  */ GameGear gears[MAX_GAME_GEARS];
    /* 0x1648 */ u8 field_0x1648[0x4];
} GameState; // size: unknown

extern GameState* g_GameState;
extern int g_GamePartyMembers[MAX_PARTY_MEMBERS];

#endif