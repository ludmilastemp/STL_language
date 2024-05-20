#ifndef STL_Gen_
#define STL_Gen_

#include <math.h>
#include <string.h>
#include <assert.h>

/// это скорее доки
const int ValueSize = 4; /// in bytes
const int DispSize  = 1; /// in bytes 

typedef char Reg_t;
typedef char Modrm_mod_t;
typedef char Modrm_rm_t;

enum Prefixs
{
    PREFIX_REX = 0x48,
};

enum Opcodes
{
    OPCODE_PUSH_R          = 0x50,  
    OPCODE_POP_R           = 0x58,  
    OPCODE_MOV_R_TO_RM     = 0x8b,  
    OPCODE_MOV_RM_TO_R     = 0x89,  
    OPCODE_MOV_RM_TO_IMM   = 0xc7,
    OPCODE_MOV_IMM_TO_R    = 0xb8,  
    OPCODE_RET_NEAR        = 0xc3,  
    OPCODE_ADD_R_TO_RM     = 0x01,  
    OPCODE_SUB_R_FROM_RM   = 0x29, 
    OPCODE_ADD_SUB_R_IMM   = 0x83,
    OPCODE_CONST_ADD_R_IMM = 0x00,
    OPCODE_CONST_SUB_R_IMM = 0x05,
    OPCODE_MUL_RM          = 0xf7,  
    OPCODE_XOR_RM_XOR_R    = 0x31,   
    OPCODE_CMP_R_WITH_RM   = 0x39,   
    OPCODE_JMP_NEAR        = 0xe9, 
    OPCODE_CALL_NEAR_REL   = 0xe8,
    OPCODE_CALL_ABS        = 0xFF,
    OPCODE_CALL_K_NEAR_ABS = 0x02,

    OPCODE_JCC_JA          = 0x77,
    OPCODE_JCC_JAE         = 0x73,
    OPCODE_JCC_JB          = 0x72,
    OPCODE_JCC_JBE         = 0x76,
    OPCODE_JCC_JE          = 0x74,
    OPCODE_JCC_JNE         = 0x75,
};

enum Registers
{
    REG_RAX = 0x00,
    REG_RCX = 0x01,
    REG_RDX = 0x02,
    REG_RBX = 0x03,
    REG_RSP = 0x04,
    REG_RBP = 0x05,
    REG_RSI = 0x06,
    REG_RDI = 0x07,
};

enum ModrmMod 
{
    MODRM_MOD_MEM        = 0x00,
    MODRM_MOD_MEM_DISP8  = 0x01,
    MODRM_MOD_MEM_DISP32 = 0x02,
    MODRM_MOD_RM         = 0x03,
};


//--------------------------------------------------------
/// GenPush (reg)               -||- push reg
/// GenPop (reg)                -||- pop reg
/// GenMovRR (reg1, reg2)       -||- reg1 = reg2
/// GenMovRM (reg, regM, disp)  -||- reg = [regM + disp]
/// GenMovMR (reg, regM, disp)  -||- [regM + disp] = reg
/// GenMovRMIMM (regM, disp, value) -||- [regM + disp] = value
/// GenMovRADD (reg, value)     -||- reg = &value
/// GenRet ()                   -||- ret
/// GenAdd (reg1, reg2)         -||- reg1 = reg1 + reg2
/// GenSub (reg1, reg2)         -||- reg1 = reg1 - reg2
/// GenMul (regM, disp)         -||- rax  = rax  * [regM + disp]
/// GenXor (reg1, reg2)         -||- reg1 = reg1 ^ reg2
/// GenCmp (reg1, reg2)         -||- cmp (reg1, reg2)

void GenValue    (char** buffer, int value);
void GenDisp     (char** buffer, int disp);
void GenPush     (char** buffer, Reg_t reg);
void GenPop      (char** buffer, Reg_t reg);
void GenMovRR    (char** buffer, Reg_t reg1, Reg_t reg2);
void GenMovRM    (char** buffer, Reg_t reg, Reg_t regM, int disp);
void GenMovMR    (char** buffer, Reg_t reg, Reg_t regM, int disp);
void GenMovRMIMM (char** buffer, Reg_t regM, int disp, int value);
void GenMovRADD  (char** buffer, Reg_t reg, char* value);
void GenMovRIMM  (char** buffer, Reg_t reg, int value);
void GenCallAbs  (char** buffer, Reg_t reg);
void GenRet      (char** buffer);
void GenAdd      (char** buffer, Reg_t reg1, Reg_t reg2);
void GenSub      (char** buffer, Reg_t reg1, Reg_t reg2);
void GenAddRIMM  (char** buffer, Reg_t reg, int value);
void GenSubRIMM  (char** buffer, Reg_t reg, int value);
void GenMul      (char** buffer, Reg_t regM, int disp);
void GenXor      (char** buffer, Reg_t reg1, Reg_t reg2);
void GenCmp      (char** buffer, Reg_t reg1, Reg_t reg2);

int MySqrt (int x);

#endif /* STL_Gen_ */
