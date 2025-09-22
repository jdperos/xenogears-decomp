#include "common.h"

#include "libspu_internal.h"

void _spu_setReverbAttr(ReverbPreset* preset) {
    s32 Mask;
    s32 bSetAllAttribues;

    Mask = preset->m_Mask;
    bSetAllAttribues = Mask == 0;
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dAPF1)) {
        _spu_RXX->_rxx.m_Reverb.m_dAPF1 = preset->m_Regs.m_dAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dAPF2)) {
        _spu_RXX->_rxx.m_Reverb.m_dAPF2 = preset->m_Regs.m_dAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vIIR)) {
        _spu_RXX->_rxx.m_Reverb.m_vIIR = preset->m_Regs.m_vIIR;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB1)) {
        _spu_RXX->_rxx.m_Reverb.m_vCOMB1 = preset->m_Regs.m_vCOMB1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB2)) {
        _spu_RXX->_rxx.m_Reverb.m_vCOMB2 = preset->m_Regs.m_vCOMB2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB3)) {
        _spu_RXX->_rxx.m_Reverb.m_vCOMB3 = preset->m_Regs.m_vCOMB3;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB4)) {
        _spu_RXX->_rxx.m_Reverb.m_vCOMB4 = preset->m_Regs.m_vCOMB4;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vWALL)) {
        _spu_RXX->_rxx.m_Reverb.m_vWALL = preset->m_Regs.m_vWALL;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vAPF1)) {
        _spu_RXX->_rxx.m_Reverb.m_vAPF1 = preset->m_Regs.m_vAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vAPF2)) {
        _spu_RXX->_rxx.m_Reverb.m_vAPF2 = preset->m_Regs.m_vAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLSAME)) {
        _spu_RXX->_rxx.m_Reverb.m_mLSAME = preset->m_Regs.m_mLSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRSAME)) {
        _spu_RXX->_rxx.m_Reverb.m_mRSAME = preset->m_Regs.m_mRSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB1)) {
        _spu_RXX->_rxx.m_Reverb.m_mLCOMB1 = preset->m_Regs.m_mLCOMB1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB1)) {
        _spu_RXX->_rxx.m_Reverb.m_mRCOMB1 = preset->m_Regs.m_mRCOMB1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB2)) {
        _spu_RXX->_rxx.m_Reverb.m_mLCOMB2 = preset->m_Regs.m_mLCOMB2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB2)) {
        _spu_RXX->_rxx.m_Reverb.m_mRCOMB2 = preset->m_Regs.m_mRCOMB2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dLSAME)) {
        _spu_RXX->_rxx.m_Reverb.m_dLSAME = preset->m_Regs.m_dLSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dRSAME)) {
        _spu_RXX->_rxx.m_Reverb.m_dRSAME = preset->m_Regs.m_dRSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLDIFF)) {
        _spu_RXX->_rxx.m_Reverb.m_mLDIFF = preset->m_Regs.m_mLDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRDIFF)) {
        _spu_RXX->_rxx.m_Reverb.m_mRDIFF = preset->m_Regs.m_mRDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB3)) {
        _spu_RXX->_rxx.m_Reverb.m_mLCOMB3 = preset->m_Regs.m_mLCOMB3;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB3)) {
        _spu_RXX->_rxx.m_Reverb.m_mRCOMB3 = preset->m_Regs.m_mRCOMB3;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB4)) {
        _spu_RXX->_rxx.m_Reverb.m_mLCOMB4 = preset->m_Regs.m_mLCOMB4;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB4)) {
        _spu_RXX->_rxx.m_Reverb.m_mRCOMB4 = preset->m_Regs.m_mRCOMB4;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dLDIFF)) {
        _spu_RXX->_rxx.m_Reverb.m_dLDIFF = preset->m_Regs.m_dLDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dRDIFF)) {
        _spu_RXX->_rxx.m_Reverb.m_dRDIFF = preset->m_Regs.m_dRDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLAPF1)) {
        _spu_RXX->_rxx.m_Reverb.m_mLAPF1 = preset->m_Regs.m_mLAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRAPF1)) {
        _spu_RXX->_rxx.m_Reverb.m_mRAPF1 = preset->m_Regs.m_mRAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLAPF2)) {
        _spu_RXX->_rxx.m_Reverb.m_mLAPF2 = preset->m_Regs.m_mLAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRAPF2)) {
        _spu_RXX->_rxx.m_Reverb.m_mRAPF2 = preset->m_Regs.m_mRAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vLIN)) {
        _spu_RXX->_rxx.m_Reverb.m_vLIN = preset->m_Regs.m_vLIN;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vRIN)) {
        _spu_RXX->_rxx.m_Reverb.m_vRIN = preset->m_Regs.m_vRIN;
    }
}