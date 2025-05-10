#include "common.h"
#include "field/main.h"

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80078B5C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80078BC8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80078C5C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80078D44);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80079288);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007954C);

void func_800796F4(void) {
}

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", FieldSwapRenderContext);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80079784);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800798BC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007995C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", FieldRenderSyncAndFlush);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800799D4);

void func_8007A44C(POLY_FT4* poly, short u0, short v0, short u1, short v1, short u2, short v2, short u3, short v3) {
    if (u0 < 0) u0 = 0;
    if (u1 < 0) u1 = 0;
    if (u2 < 0) u2 = 0;
    if (u3 < 0) u3 = 0;
    if (v0 < 0) v0 = 0;
    if (v1 < 0) v1 = 0;
    if (v2 < 0) v2 = 0;
    if (v3 < 0) v3 = 0;
    
    if (u0 >= 0x100) u0 = 0xFF;
    if (u1 >= 0x100) u1 = 0xFF;
    if (u2 >= 0x100) u2 = 0xFF;
    if (u3 >= 0x100) u3 = 0xFF;
    if (v0 >= 0x100) v0 = 0xFF;
    if (v1 >= 0x100) v1 = 0xFF;
    if (v2 >= 0x100) v2 = 0xFF;
    if (v3 >= 0x100) v3 = 0xFF;

    poly->u0 = u0;
    poly->v0 = v0;
    poly->u1 = u1;
    poly->v1 = v1;
    poly->u2 = u2;
    poly->v2 = v2;
    poly->u3 = u3;
    poly->v3 = v3;
}

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007A5C4);

//INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007A7F4);

extern SVec4 D_800ADCE0[]; // X0 -> X3 positions
extern SVec4 D_800ADD28[]; // Y0 -> Y3 positions
extern SVec4 D_800ADD70[]; // U0 -> U3s
extern SVec4 D_800ADDB8[]; // V0 -> V3s

// Texture / Clut stuff
extern short D_800ADE00[];
extern short D_800ADE02[];
extern short D_800ADE04[];
extern short D_800ADE06[];
extern short D_800ADE08[];
extern short D_800ADE0A[];

void func_8007A7F4(Quad* pPart, int x, int y, int tex) {
    int nIndexTex;
    POLY_FT4* pNextPoly;
    char _pad[0x88];

    pNextPoly = &pPart->polys[1]; 
    SetPolyFT4(&pPart->polys[0]);
    
    nIndexTex = tex * 0x6,

    // Set Position 0, 1, 2, and 3
    pPart->vecs[0].vx = D_800ADCE0[x].x;
    pPart->vecs[0].vy = 0x0;
    pPart->vecs[0].vz = D_800ADD28[y].x;

    pPart->vecs[1].vx = D_800ADCE0[x].y;
    pPart->vecs[1].vy = 0;
    pPart->vecs[1].vz = D_800ADD28[y].y;

    pPart->vecs[2].vx = D_800ADCE0[x].z;
    pPart->vecs[2].vy = 0;
    pPart->vecs[2].vz = D_800ADD28[y].z;

    pPart->vecs[3].vx = D_800ADCE0[x].w;
    pPart->vecs[3].vy = 0;
    pPart->vecs[3].vz = D_800ADD28[y].w;
    
    setRGB0(&pPart->polys[0], 0x80, 0x80, 0x80);
    
    pPart->polys[0].tpage = GetTPage(
        D_800ADE00[nIndexTex], 
        D_800ADE02[nIndexTex], 
        D_800ADE04[nIndexTex], 
        D_800ADE06[nIndexTex]
    );
    
    pPart->polys[0].clut = GetClut(D_800ADE08[nIndexTex], D_800ADE0A[nIndexTex]);
    
    func_8007A44C(&pPart->polys[0], 
        D_800ADD70[x].x, D_800ADDB8[y].x, 
        D_800ADD70[x].y, D_800ADDB8[y].y, 
        D_800ADD70[x].z, D_800ADDB8[y].z, 
        D_800ADD70[x].w, D_800ADDB8[y].w
    );

    *pNextPoly = pPart->polys[0];
}


INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007AA44);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007AB6C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007AC58);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", FieldSetControllerBuffers);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007ADA4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007AE14);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007AE2C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007AE78);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007AF74);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007B07C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007B1C4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007B478);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007B614);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007B694);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007B6C4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007B814);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007BAC0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007BEF4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007C670);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007C694);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007CD3C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007CD60);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007CD80);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007D3D4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007D818);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007D8B4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007D93C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007DA44);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007DCF8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007DECC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007E114);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007E16C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007E1C0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007EE0C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007F5AC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007F6F8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007F814);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007F8DC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007FFE8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8008004C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800805F4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800806E4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80080720);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80080760);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800807B4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8008083C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80080968);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800809D0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80080A18);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80080A74);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80080F44);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8008110C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800815F0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80081C54);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80081F5C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80081F80);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800821F4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8008237C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80082494);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800825AC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80082620);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80082BB8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80083178);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800831D0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800831F4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80083288);

void func_80083994(void) {
}

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8008399C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80084158);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8008492C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80084A40);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800854D0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085560);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800855C8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085634);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085678);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085738);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085788);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085890);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085988);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800859DC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085B20);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085C3C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085C90);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085EEC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085F30);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80085FB8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086024);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086078);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800860F0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086200);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800862CC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800863E8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086470);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800864B4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800864F0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086590);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086908);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800869B8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086A1C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086BA8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086C34);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80086D4C);
