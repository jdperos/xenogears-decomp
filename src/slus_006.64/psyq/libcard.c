#include "common.h"

#include "psyq/libapi.h"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcard", _card_info);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcard", InitCARD);

//INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcard", StartCARD);
void StartCARD(void) {
  EnterCriticalSection();
  StartCARD2();
  ChangeClearPAD(0);
  ExitCriticalSection();
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcard", StopCARD);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcard", InitCARD2);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcard", StartCARD2);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcard", StopCARD2);
