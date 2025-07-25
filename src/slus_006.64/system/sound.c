#include "common.h"
#include "system/sound.h"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundInitialize);

extern long D_800595BC; // Unknown event descriptor
void SoundReset(void) {
    int i;

    if (g_SoundControlFlags == 0x0) {
        SoundHandleError(0x29);
        return;
    }
    
    EnterCriticalSection();
    g_SoundControlFlags = 0;
    SpuSetIRQ(SPU_OFF);
    SpuSetTransferCallback(NULL);
    SpuSetIRQCallback(NULL);
    func_800406FC(0xF2000002);
    CloseEvent(D_800595BC);
    ExitCriticalSection();
    for (i = 0; i < 0x18; i++) {
        func_8003F5BC(i, 6, 3);
    }
    SoundSetVoiceKeyOff(0xFFFFFF); // Release all voices
    SpuSetReverbModeDepth(0, 0);
    SpuSetReverbModeType(0);
    g_SoundSpuErrorId = 0;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundEnableAllSpuChannels);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundMuteAllSpuChannels);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80037F44);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80037F88);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80037FD8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800380D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800381F4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038264);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003827C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038310);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800383EC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038428);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003852C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038624);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003864C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003869C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800386C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038824);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003885C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800388D4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003890C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038934);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038AD4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038B4C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038C68);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038D18);

void func_80038DB4(long reverb, long mix) {
    g_SoundSpuCommonAttr.cd.reverb = reverb;
    g_SoundSpuCommonAttr.cd.mix = mix;
    g_SoundSpuCommonAttr.mask = g_SoundSpuCommonAttr.mask | SPU_COMMON_CDREV | SPU_COMMON_CDMIX;
    SpuSetCommonAttr(&g_SoundSpuCommonAttr);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038DF4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038E6C);
/*
Matches on GCC 2.7.2-970404 + ASPSX 2.63

void func_80038E6C(s16 volume, SpuVolume* pVolume, u16 arg2) {
    s16 var_v0;
    s32 temp_a2;

    pVolume->right = volume;
    pVolume->left = volume;
    
    if (g_SoundControlFlags & 0x600) {
        temp_a2 = arg2 & 0xFF;
        if (!(g_SoundControlFlags & 0x200)) {
            if ((temp_a2 ^ 1) != 0) {
                pVolume->left = -volume;
            } else {
                pVolume->right = -volume;
            }
            return;
        }

        if (temp_a2) {
            pVolume->left = -volume;
        } else {
            pVolume->right = -volume;
        }
    }  
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038EC0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038F18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039024);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039144);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800391CC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039248);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800392EC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039360);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800393B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800394B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800395B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800396E0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039748);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003977C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039784);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800397C0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800397FC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039850);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039910);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800399D4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039A80);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039B68);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039C4C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039C8C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039CC4);

void func_80039D24(void) {}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039D2C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039D78);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039DB8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039E18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039E60);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039EC4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039F18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039F9C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039FF8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A094);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A14C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A20C);

void func_8003A2D4(void) {}

void func_8003A2DC(void) {}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A2E4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A344);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A3B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A450);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A4FC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A55C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A5D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A65C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A82C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A838);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A89C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A948);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A9BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AA30);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AAC4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003ABE8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003ABF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AC58);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003ACC8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AD20);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AD98);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003ADCC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AE84);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AF24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AFA0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AFFC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B060);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B0AC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B148);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B1FC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B22C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B32C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B370);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B424);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B644);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B930);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B97C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B9E4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BA38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BB08);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BB40);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BB64);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BC10);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BC34);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BC58);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BC7C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BCA0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BDBC);

void func_8003BDF4(void) {}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BDFC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BE68);

void SoundSpuIRQHandler(void) {
    g_SoundControlFlags |= 4;
    g_SoundSpuIRQCount++;
    if (g_SoundSpuIrqCallbackFn) {
        g_SoundSpuIrqCallbackFn();
    }
    g_SoundControlFlags &= 0xFFFB;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003C010);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003C020);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003C484);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003C4C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003C6E8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CC84);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD00);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD08);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD30);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD4C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD54);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD7C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD84);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD8C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE04);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE50);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE68);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE9C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CEC0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CED4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CEF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CF38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CFA4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CFF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D034);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D070);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D0E8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D110);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D13C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D17C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D1BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D208);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D21C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D298);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D2D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D300);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D328);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D340);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D358);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D370);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D3A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D3D8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D438);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D4A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D4C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D4E4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D53C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D59C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D5BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D5C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D5CC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D5D4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D60C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D640);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D65C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D678);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D694);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D6B4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D6D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D6F8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D714);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D730);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D74C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D770);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D79C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D7C8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D7FC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D854);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D86C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D884);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D8B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D9A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DAB0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DAEC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DB0C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DB2C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DB58);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DB98);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DBE4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DC50);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DD24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DE18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DE54);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DE74);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DE94);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DEB4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DEE4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DF3C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DF78);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E04C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E140);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E160);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E180);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E1F8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E290);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E308);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E358);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E360);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E3E0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E40C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E44C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E4BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E4F0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E54C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E5BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E680);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E6C0);

void SoundClearVoiceDataPointers(void) {
    s32 offset = sizeof(SoundVoiceData*) * (NUM_VOICES - 1);
    while( offset >= 0 ) {
        *(u32*)((u8*)&g_SoundVoiceDataPointerArray + offset) = NULL;
        offset -= sizeof(SoundVoiceData*);
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E724);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E7E0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E83C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E8A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E900);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EB5C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EBF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EEA0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EF04);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EFA0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EFE4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F190);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F1A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F1EC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F240);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F2A0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F308);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F354);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F3C0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F42C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F43C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F468);

// Set the SPU_VOICE_KEY_OFF register, which will release / fade out voices according to the flags
void SoundSetVoiceKeyOff(unsigned int voiceFlags) {
    unsigned short* pSoundRegisters = g_pSoundSpuRegisters;
    // TODO: Clean up this
    *(pSoundRegisters + 0xC6) = voiceFlags;
    *(pSoundRegisters + 0xC7) = (voiceFlags >> 0x10);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F4A0);

void func_8003F4BC(void) {}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F4C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F4E0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F4FC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F518);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F530);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F560);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F588);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F5BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F5EC);

//
int SoundValidateFile(SoundFile* pSoundFile, u32 magicBytes, unsigned short targetValue) {
    unsigned char bIsError;
    
    if (pSoundFile->magic != magicBytes) {
        return SOUND_ERR_INVALID_SIGNATURE;
    }
    
    if (SoundFileComputeChecksum(pSoundFile) == 0) {
        // Version check?
        bIsError = (pSoundFile->unkC != targetValue);
        return bIsError * SOUND_ERR_UNK_0X4;
    }
    
    return SOUND_ERR_INVALID_CHECKSUM;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F67C);

int SoundFileComputeChecksum(SoundFile* pSoundFile) {
    int nResult;
    int* pCurrent;
    unsigned int nCount;

    pCurrent = pSoundFile;
    nCount = (pSoundFile->unk8 + 3) / 4; // Align to 4-byte boundary
    nResult = 0;
    do {
        nResult += *pCurrent++;
    } while (--nCount);
    
    return nResult;
}


INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundHandleError);
