#include "common.h"
#include "system/controller.h"

int ControllerGetButtonState(int controllerIndex) {
    int nType;
    u_char* pController;

    pController = &D_800625FC[controllerIndex * CONTROLLER_BUFFER_SIZE];
    g_ControllerType = 0;
    if (pController[CONTROLLER_STATUS] == CONTROLLER_STATUS_SUCCESS) {
        g_ControllerType = pController[CONTROLLER_TYPE] & 0xF0;
        if (g_ControllerType == CONTROLLER_INTERNAL_TYPE_DIGITAL_PAD || 
            g_ControllerType == CONTROLLER_INTERNAL_TYPE_ANALOG_PAD  || 
            g_ControllerType == CONTROLLER_INTERNAL_TYPE_ANALOG_STICK
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

short ControllerRemapButtonState(u_short buttonState) {
    u_short nMaskedState;
    int i;

    nMaskedState = buttonState & (
        CTRL_BTN_SELECT | CTRL_BTN_L3 | CTRL_BTN_R3 | CTRL_BTN_START |
        CTRL_BTN_UP | CTRL_BTN_RIGHT | CTRL_BTN_DOWN | CTRL_BTN_LEFT
    );
    
    for (i = 0; i < 8; i++) {
        if (g_ControllerButtonMasks[i] & buttonState) {
            nMaskedState |= g_ControllerButtonMasks[g_ControllerButtonMappings[i]];
        }
    };
    
    return nMaskedState;
}

// Analog Stick Controller (0x5A53) has some buttons rearranged.
// This function remaps these buttons so we have a uniform state
short ControllerRemapAnalogJoystickState(short buttonState) {
    short nMaskedState;

    // Mask out buttons which are rearranged on analog joystick controllers
    nMaskedState = buttonState & ~(
        CTRL_BTN_R2 | CTRL_BTN_L1 | CTRL_BTN_R1 | CTRL_BTN_TRIANGLE | CTRL_BTN_SQUARE
    );

    // Remap them
    if (buttonState & CTRL_ANALOG_STICK_BTN_TRIANGLE) {
        nMaskedState |= nMaskedState | CTRL_BTN_TRIANGLE;
    }
    if (buttonState & CTRL_ANALOG_STICK_BTN_L1) {
        nMaskedState |= CTRL_BTN_L1;
    }
    if (buttonState & CTRL_ANALOG_STICK_BTN_R1) {
        nMaskedState |= CTRL_BTN_R1;
    }
    if (buttonState & CTRL_ANALOG_STICK_BTN_R2) {
        nMaskedState |= CTRL_BTN_R2;
    }
    if (buttonState & CTRL_ANALOG_STICK_BTN_SQUARE) {
        nMaskedState |= CTRL_BTN_SQUARE;
    }
    return nMaskedState;
}

u_char ControllerStickToAnalogX(int buttonState) {
    return g_ControllerStickToAnalogX[((buttonState >> 0xC) & 0xF)];
}

u_char ControllerStickToAnalogY(int buttonState) {
    return g_ControllerStickToAnalogY[((buttonState >> 0xC) & 0xF)];
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", ControllerPoll);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_80035C0C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_80035CDC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", func_80035DA0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/controller", ControllerResetState);
