#include "common.h"

#include "libspu_internal.h"

void SpuSetReverbModeDepth(short DepthL, short DepthR)
{
    short* RL = &_spu_RXX->_rxx.rev_vol.left;
    short* RR = &_spu_RXX->_rxx.rev_vol.right;
    *RL = DepthL;
    *RR = DepthR;
    _spu_rev_attr.depth.left = DepthL;
    _spu_rev_attr.depth.right = DepthR;
}