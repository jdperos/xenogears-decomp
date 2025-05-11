#include "common.h"
#include "field/main.h"
#include "psyq/libetc.h"

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80078B5C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80078BC8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80078C5C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80078D44);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80079288);

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007954C);

void func_800796F4(void) {
}

void FieldSwapRenderContext(void) {
    g_FieldCurRenderContextIndex = (g_FieldCurRenderContextIndex + 1) % 2;
    g_FieldCurRenderContext = &g_FieldRenderContexts[g_FieldCurRenderContextIndex];
    PutDispEnv(&g_FieldCurRenderContext->dispEnv);
    PutDrawEnv(&g_FieldCurRenderContext->drawEnvs[0]);
}

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_80079784);
/*
Almost matches, some weird stuff going on when setting color

extern RECT D_800AFE4C;
extern TILE D_800AFE54[];
extern DR_MODE D_800AFE24[];

void func_80079784(int color) {
    u_char nColor;
    TILE* pTile;

    FieldClearAndSwapOTag();
    pTile = &D_800AFE54[g_FieldCurRenderContextIndex];
    nColor = color * 4;
    
    setRGB0(pTile, nColor, nColor, nColor);

    addPrim(g_FieldCurRenderContext->ot1, &D_800AFE54[g_FieldCurRenderContextIndex]);
    addPrim(g_FieldCurRenderContext->ot1, &D_800AFE24[g_FieldCurRenderContextIndex]);

    FieldRenderSync();
    MoveImage(&D_800AFE4C, 0, g_FieldCurRenderContextIndex * 0x100);
    PutDispEnv(&g_FieldCurRenderContext->dispEnv);
    PutDrawEnv(&g_FieldCurRenderContext->drawEnvs[0]);
    DrawOTag(g_FieldCurRenderContext->ot1 + 1);
}
*/

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800798BC);

void func_8007995C(short w, short h, short x, short y, int destX, int destY) {
    RECT rect;

    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;
    MoveImage(&rect, destX, destY);
    DrawSync(0);
}

void FieldRenderSyncAndFlush(void) {
    FieldRenderSync();
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_800799D4);


// Quad rendering
// --------------------

// Set POLY_FT4 UVs
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

// Compass primitive initialization?
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

// Actor primitive initialization
void func_8007AA44(Quad* pQuad) {
    POLY_FT4* pPoly;
    POLY_FT4* pPoly2;

    pPoly = &pQuad->polys[0];
    pPoly2 = &pQuad->polys[1];
    
    SetPolyFT4(pPoly);
    pQuad->vecs[3].vx = -0x18;
    pQuad->vecs[3].vz = -0x18;
    pQuad->vecs[3].vy = 0;

    pQuad->vecs[2].vx = 0x18;
    pQuad->vecs[2].vy = 0;
    pQuad->vecs[2].vz = -0x18;

    pQuad->vecs[1].vx = -0x18;
    pQuad->vecs[1].vy = 0;
    pQuad->vecs[1].vz = 0x18;

    pQuad->vecs[0].vx = 0x18;
    pQuad->vecs[0].vy = 0;
    pQuad->vecs[0].vz = 0x18;

    setRGB0(pPoly, 0x80, 0x80, 0x80);
    pPoly->tpage = GetTPage(0, 2, 0x280, 0x1E0);
    pPoly->clut = GetClut(0x100, 0xF3);
    SetSemiTrans(pPoly, 1);

    pPoly->v0 = 0xE0;
    pPoly->v1 = 0xE0;
    pPoly->u0 = 0;
    pPoly->u1 = 0xF;
    pPoly->u2 = 0;
    pPoly->v2 = 0xEF;
    pPoly->u3 = 0xF;
    pPoly->v3 = 0xEF;
    
    *pPoly2 = *pPoly;
}

// Draw Quad / Actor primitive
void func_8007AB6C(u_long* ot, Quad* pQuad, MATRIX* matrix, int renderContext) {
    long nInterpolation;
    long nFlag;
    POLY_FT4* pPoly;

    pPoly = &pQuad->polys[renderContext];
    PushMatrix();
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    RotAverage4(
        &pQuad->vecs[0], &pQuad->vecs[1], &pQuad->vecs[2], &pQuad->vecs[3], 
        &pPoly->x0, &pPoly->x1, &pPoly->x2, &pPoly->x3, 
        &nInterpolation, &nFlag
    );
    addPrim(ot + 1, pPoly);
    PopMatrix();
}


INCLUDE_ASM("asm/field/nonmatchings/main/misc4", func_8007AC58);

// Controller stuff
// --------------------
extern s32 g_pFieldControllerBuffer1;
extern s32 g_pFieldControllerBuffer2;

extern u_short g_FieldMouseSpeedX;
extern u_short g_FieldMouseSpeedY;
extern s32 D_800C3A44;
extern s32 D_800C3A4C;
extern s32 D_800C3A50;
extern s32 D_800C3A54;

void FieldSetControllerBuffers(void* controllerBuffer1, void* controllerBuffer2) {
    g_pFieldControllerBuffer1 = controllerBuffer1;
    g_pFieldControllerBuffer2 = controllerBuffer2;
}

void func_8007ADA4(int arg0, int arg1, int arg2, int arg3) {
    D_800C3A44 = arg0 * g_FieldMouseSpeedX;
    D_800C3A50 = arg1 * g_FieldMouseSpeedX;
    D_800C3A4C = arg2 * g_FieldMouseSpeedY;
    D_800C3A54 = arg3 * g_FieldMouseSpeedY;
}

void func_8007AE14(u_short xSpeed, u_short ySpeed) {
    g_FieldMouseSpeedX = xSpeed;
    g_FieldMouseSpeedY = ySpeed;
}

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
