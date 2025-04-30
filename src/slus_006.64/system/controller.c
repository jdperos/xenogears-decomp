#include "common.h"
#include "common.h"
#include "system/controller.h"

int ControllerGetButtonState(int controllerIndex) {
    int nType;
    u_char* pController;

    pController = &D_800625FC[controllerIndex * CONTROLLER_BUFFER_SIZE];
    D_80059388 = 0;
    if (pController[CONTROLLER_STATUS] == CONTROLLER_STATUS_SUCCESS) {
        D_80059388 = pController[CONTROLLER_TYPE] & 0xF0;
        if (D_80059388 == CONTROLLER_INTERNAL_TYPE_DIGITAL_PAD || 
            D_80059388 == CONTROLLER_INTERNAL_TYPE_ANALOG_PAD  || 
            D_80059388 == CONTROLLER_INTERNAL_TYPE_ANALOG_STICK
        ) {
            return (
                ~pController[CONTROLLER_BUTTONS_2] & 0xFF) | 
                ((pController[CONTROLLER_BUTTONS_1] << 8) ^ 0xFF00);
        }
    }
    return 0;
}

int ControllerGetType(int controllerIndex) {
    int nController;
    int nType;

    nController = controllerIndex * CONTROLLER_BUFFER_SIZE;
    if (D_800625FC[nController] != CONTROLLER_STATUS_ERROR) {
        nType = D_800625FD[nController] & 0xF0;
        switch (nType) {
            case CONTROLLER_INTERNAL_TYPE_DIGITAL_PAD:
                return CONTROLLER_TYPE_DIGITAL_PAD;
            case CONTROLLER_INTERNAL_TYPE_MOUSE:
                return CONTROLLER_TYPE_MOUSE;
            case CONTROLLER_INTERNAL_TYPE_ANALOG_PAD:
                return CONTROLLER_TYPE_ANALOG_PAD;
            case CONTROLLER_INTERNAL_TYPE_ANALOG_STICK:
                return CONTROLLER_TYPE_ANALOG_STICK;
            default:
                return CONTROLLER_TYPE_ERROR;
        }
    }
    
    return CONTROLLER_TYPE_NONE;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_800357C0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_8003582C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_80035884);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_800358A0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_800358BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_80035C0C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_80035CDC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_80035DA0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", ControllerResetState);
