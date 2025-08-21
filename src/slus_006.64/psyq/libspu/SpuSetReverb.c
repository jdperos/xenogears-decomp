#include "common.h"

#include "libspu_internal.h"

long SpuSetReverb (long on_off)
{
    long spuControlRegister;

    switch( on_off ) {
        case SPU_OFF:
            _spu_rev_flag = SPU_OFF;
            spuControlRegister = _spu_RXX->_rxx.spucnt;
            spuControlRegister &= ~SPU_CTRL_MASK_REVERB_MASTER_ENABLE;
            _spu_RXX->_rxx.spucnt = spuControlRegister;
            break;

        case SPU_ON:
            if( _spu_rev_reserve_wa != on_off && _SpuIsInAllocateArea_(_spu_rev_offsetaddr) ) {
                _spu_rev_flag = SPU_OFF;
                spuControlRegister = _spu_RXX->_rxx.spucnt;
                spuControlRegister &= ~SPU_CTRL_MASK_REVERB_MASTER_ENABLE;
                _spu_RXX->_rxx.spucnt = spuControlRegister;
            } else {
                _spu_rev_flag = on_off;
                spuControlRegister = _spu_RXX->_rxx.spucnt;
                spuControlRegister |= SPU_CTRL_MASK_REVERB_MASTER_ENABLE;
                _spu_RXX->_rxx.spucnt = spuControlRegister;
            }
            break;
    }

    return _spu_rev_flag;
}