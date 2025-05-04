#include "common.h"

void* memmove(u_char* pDst, u_char* pSrc, int size) {
    if (pDst >= pSrc) {
        while (size-- > 0) {
            pDst[size] = pSrc[size];
        }
    } else {
        while (size-- > 0) {
            *pDst++ = *pSrc++;
        }
    }

    return pDst;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", func_8003FA08);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", func_8003FA38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", func_8003FA68);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", func_8003FA78);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", func_8003FB20);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", func_8003FB84);

int strlen(char* pString) {
    int nLen;
    char chCur;

    nLen = 0;
    if (pString == NULL)
        return 0;

    while (chCur = *pString, pString++, chCur != NULL)
        nLen++;
    return nLen;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", Sprintf);
