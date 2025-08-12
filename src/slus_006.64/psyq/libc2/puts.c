#include "common.h"
#include "psyq/stdio.h"

extern u32 D_8005A234;
extern u8 D_800567C1[];
extern char* D_80018F3C; // "<NULL>"

void puts(char* str) {
    char ch;
    if (!str)
        str = (char*)&D_80018F3C;

    while ((ch = *str++)) {
        putchar(ch);
    }
}

void putchar(char ch) {
    if (ch != 0x9) {
        if (ch == 0xa) {
            putchar(0xd);
            D_8005A234 = 0;
            write(1, &ch, 1);
            return;
        }
    } else {
        do {
            putchar(0x20);
        } while (D_8005A234 & 0x7);
        return;
    }

    if (D_800567C1[ch] & 0x97) {
        D_8005A234++;
    }

    write(1, &ch, 1);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc2/puts", toupper);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc2/puts", tolower);
