#include "common.h"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/graphics", GfxLoadImageAccelerated);
/*
Matches on ASPSX 2.56, GCC 2.6.0

void GfxLoadImageAccelerated(void) {
    char* pBuffer;

    pBuffer = HeapAlloc(0x2000, 1);
    
    asm(""
        "\tmove  $t0, %0\n"
        "\tsw    $sp, 0($t0)\n"
        "\taddiu $t0, $t0, -4\n"
        "\taddu  $sp, $t0, $zero\n"
    :
    : "r"(pBuffer + 0x1F00)
    : "t0", "memory"
    );
    
    LoadImage(D_800592F0, D_800592F4);
    
    asm(""
        "\taddiu $sp, $sp, 4\n"
        "\tlw    $sp, 0($sp)\n"
    ::
    : "memory"
    );
    
    HeapFree(pBuffer);
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/graphics", GfxLoadClutsAccelerated);
