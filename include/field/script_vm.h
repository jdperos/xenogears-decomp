#ifndef _XENO_FIELD_SCRIPT_VIRTUAL_MACHINE_H
#define _XENO_FIELD_SCRIPT_VIRTUAL_MACHIEN_H

#define FIELD_SCRIPT_VM_VAR_UNSIGNED -1
#define FIELD_SCRIPT_VM_VAR_SIGNED 0

#define FIELD_SCRIPT_VM_COND_EQUAL 0x0
#define FIELD_SCRIPT_VM_COND_NOT_EQUAL2 0x1
#define FIELD_SCRIPT_VM_COND_LT 0x2
#define FIELD_SCRIPT_VM_COND_LT2 0x3
#define FIELD_SCRIPT_VM_COND_GTE 0x4
#define FIELD_SCRIPT_VM_COND_GTE2 0x5
#define FIELD_SCRIPT_VM_COND_AND 0x6
#define FIELD_SCRIPT_VM_COND_NOT_EQUAL 0x7
#define FIELD_SCRIPT_VM_COND_OR 0x8
#define FIELD_SCRIPT_VM_COND_AND2 0x9
#define FIELD_SCRIPT_VM_COND_NAND 0xA

extern int FieldScriptVMGetInstructionArgument(int);
extern int FieldScriptVMGetVariableSign(int);
extern int FieldScriptVMReadArgumentFromMemory(int);
extern short func_800ACD7C(int);

#endif
