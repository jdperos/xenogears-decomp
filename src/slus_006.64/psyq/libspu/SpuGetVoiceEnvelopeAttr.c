#include "common.h"

#include "libspu_internal.h"

void SpuGetVoiceEnvelopeAttr(int vNum, long* keyStat, short* envx) {
    u_short volx;
    long offset;

    offset = (vNum * SPU_VOICE_REG_SIZE) + SPU_VOICE_REG_VOLUMEX;
    volx = _spu_RXX->raw[offset];

    *envx = volx;
    if (_spu_keystat & (1 << vNum)) {
        if ((volx << 16) > 0) {
            *keyStat = SPU_ON;
        } else {
            *keyStat = SPU_ON_ENV_OFF;
        }
    } else if ((volx << 16) > 0) {
        *keyStat = SPU_OFF_ENV_ON;
    } else {
        *keyStat = SPU_OFF;
    }
}