#include "common.h"

#include "libspu_internal.h"

void SpuGetReverbModeType(long* type)
{
    *type = _spu_rev_attr.mode;
}