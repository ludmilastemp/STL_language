#ifndef STL_Gen_
#define STL_Gen_

#include <math.h>
#include <string.h>
#include <assert.h>

typedef char Reg_t;
typedef char Modrm_mod_t;
typedef char Modrm_rm_t;

const char PREFIX_REX             = 0x48;

const char OPCODE_PUSH_R          = 0x50;  
const char OPCODE_POP_R           = 0x58;  
const char OPCODE_MOV_R_TO_RM     = 0x8b;  
const char OPCODE_MOV_RM_TO_R     = 0x89;  
const char OPCODE_MOV_RM_TO_IMM   = 0xc7;
const char OPCODE_MOV_IMM_TO_R    = 0xb8;  
const char OPCODE_RET_NEAR        = 0xc3;  
const char OPCODE_ADD_R_TO_RM     = 0x01;  
const char OPCODE_SUB_R_FROM_RM   = 0x29; 
const char OPCODE_MUL_RM          = 0xf7;  
const char OPCODE_XOR_RM_XOR_R    = 0x31;   
const char OPCODE_CMP_R_WITH_RM   = 0x39;   
const char OPCODE_JMP_NEAR        = 0xe9; 

const char OPCODE_JCC_JA          = 0x77;
const char OPCODE_JCC_JAE         = 0x73;
const char OPCODE_JCC_JB          = 0x72;
const char OPCODE_JCC_JBE         = 0x76;
const char OPCODE_JCC_JE          = 0x74;
const char OPCODE_JCC_JNE         = 0x75;

const char REG_RAX = 0x00;
const char REG_RCX = 0x01;
const char REG_RDX = 0x02;
const char REG_RBX = 0x03;
const char REG_RSP = 0x04;
const char REG_RBP = 0x05;

const char MODRM_MOD_MEM        = 0x00;
const char MODRM_MOD_MEM_DISP8  = 0x01;
const char MODRM_MOD_MEM_DISP32 = 0x02;
const char MODRM_MOD_RM         = 0x03;


//--------------------------------------------------------
/// GenPush (reg)              -||- push reg
/// GenPop (reg)               -||- pop reg
/// GenMovRR (reg1, reg2)      -||- reg1 = reg2
/// GenMovRM (reg, regM, disp) -||- reg = [regM + disp]
/// GenMovMR (reg, regM, disp) -||- [regM + disp] = reg
/// GenMovRMIMM (regM, disp, value) -||- [regM + disp] = value
/// GenMovRADD (reg, value)    -||- reg = &value
/// GenRet ()           -||- ret
/// GenAdd (reg1, reg2) -||- reg1 = reg1 + reg2
/// GenSub (reg1, reg2) -||- reg1 = reg1 - reg2
/// GenMul (regM, disp) -||- rax  = rax  * [regM + disp]
/// GenXor (reg1, reg2) -||- reg1 = reg1 ^ reg2
/// GenCmp (reg1, reg2) -||- cmp (reg1, reg2)

// const char OPCODE_JMP_NEAR      = 0xe9;

void GenValue  (char** buffer, int value);
void GenDisp   (char** buffer, int disp);
void GenPush   (char** buffer, Reg_t reg);
void GenPop    (char** buffer, Reg_t reg);
void GenMovRR  (char** buffer, Reg_t reg1, Reg_t reg2);
void GenMovRM  (char** buffer, Reg_t reg, Reg_t regM, int disp);
void GenMovMR  (char** buffer, Reg_t reg, Reg_t regM, int disp);
void GenMovRMIMM (char** buffer, Reg_t regM, int disp, int value);
void GenMovRADD  (char** buffer, Reg_t reg, char* value);
void GenMovRIMM  (char** buffer, Reg_t reg, int value);
// const char OPCODE_MOV_RM_TO_IMM = 0xc7;
void GenRet    (char** buffer);
void GenAdd    (char** buffer, Reg_t reg1, Reg_t reg2);
void GenSub    (char** buffer, Reg_t reg1, Reg_t reg2);
void GenMul    (char** buffer, Reg_t regM, int disp);
void GenXor    (char** buffer, Reg_t reg1, Reg_t reg2);
void GenCmp    (char** buffer, Reg_t reg1, Reg_t reg2);
// const char OPCODE_JMP_NEAR      = 0xe9;


int MySqrt (int x);

#endif /* STL_Gen_ */
