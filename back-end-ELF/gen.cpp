#include "gen.h"

char MakeModrm (Modrm_mod_t mod, Reg_t reg, Modrm_rm_t rm);

void GenPush (char** buffer, Reg_t reg)
{
    assert (buffer);
    assert (*buffer);

    **buffer = OPCODE_PUSH_R | reg; (*buffer)++;
}

void GenPop (char** buffer, Reg_t reg)
{
    assert (buffer);
    assert (*buffer);

    **buffer = OPCODE_POP_R | reg; (*buffer)++;
}

void GenMovRR (char** buffer, Reg_t reg1, Reg_t reg2)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                           (*buffer)++;           
    **buffer = OPCODE_MOV_RM_TO_R;                   (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_RM, reg2, reg1); (*buffer)++;
}

void GenMovRM (char** buffer, Reg_t reg, Reg_t regM, int disp)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                                 (*buffer)++;           
    **buffer = OPCODE_MOV_R_TO_RM;                         (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_MEM_DISP8, reg, regM); (*buffer)++;
    GenDisp (buffer, disp);
}

void GenMovMR (char** buffer, Reg_t reg, Reg_t regM, int disp)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                                 (*buffer)++;           
    **buffer = OPCODE_MOV_RM_TO_R;                         (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_MEM_DISP8, reg, regM); (*buffer)++;
    GenDisp (buffer, disp);
}

void GenMovRMIMM (char** buffer, Reg_t regM, int disp, int value)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                               (*buffer)++;           
    **buffer = OPCODE_MOV_RM_TO_IMM;                     (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_MEM_DISP8, 0, regM); (*buffer)++;

    GenDisp (buffer, disp);
    GenValue (buffer, value);
}

void GenMovRADD (char** buffer, Reg_t reg, char* value)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                      (*buffer)++;           
    **buffer = OPCODE_MOV_IMM_TO_R + reg;       (*buffer)++; 
    memcpy (*buffer, &value, sizeof (value));   *buffer += sizeof (value);
}

void GenMovRIMM (char** buffer, Reg_t reg, int value)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                      (*buffer)++;           
    **buffer = OPCODE_MOV_IMM_TO_R + reg;       (*buffer)++; 
    GenValue (buffer, value);                   *buffer += 8 - sizeof (value);
}

void GenCallAbs (char** buffer, Reg_t reg)
{
    assert (buffer);
    assert (*buffer);

    **buffer = OPCODE_CALL_ABS; (*buffer)++;
    **buffer = MakeModrm (MODRM_MOD_RM, OPCODE_CALL_K_NEAR_ABS, reg); (*buffer)++;
}

void GenRet (char** buffer)
{
    assert (buffer);
    assert (*buffer);

    **buffer = OPCODE_RET_NEAR; (*buffer)++;
}

void GenAdd (char** buffer, Reg_t reg1, Reg_t reg2)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                           (*buffer)++;           
    **buffer = OPCODE_ADD_R_TO_RM;                   (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_RM, reg2, reg1); (*buffer)++;
}
                    
void GenSub (char** buffer, Reg_t reg1, Reg_t reg2)
{
    assert (buffer);
    assert (*buffer);

    **buffer = PREFIX_REX;                           (*buffer)++;           
    **buffer = OPCODE_SUB_R_FROM_RM;                 (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_RM, reg2, reg1); (*buffer)++;
}
                    
void GenAddRIMM (char** buffer, Reg_t reg, int value)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                           (*buffer)++;           
    **buffer = OPCODE_ADD_SUB_R_IMM;                 (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_RM, OPCODE_CONST_ADD_R_IMM, reg); (*buffer)++;
    GenDisp (buffer, value);
}          

void GenSubRIMM (char** buffer, Reg_t reg, int value)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                           (*buffer)++;           
    **buffer = OPCODE_ADD_SUB_R_IMM;                 (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_RM, OPCODE_CONST_SUB_R_IMM, reg); (*buffer)++;
    GenDisp (buffer, value);
}
                    
void GenMul (char** buffer, Reg_t regM, int disp)
{
    assert (buffer);
    assert (*buffer);

    **buffer = PREFIX_REX;                               (*buffer)++;           
    **buffer = OPCODE_MUL_RM;                            (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_MEM_DISP8, 4, regM); (*buffer)++;
    GenDisp (buffer, disp);
}
                    
void GenXor (char** buffer, Reg_t reg1, Reg_t reg2)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                           (*buffer)++;           
    **buffer = OPCODE_XOR_RM_XOR_R;                  (*buffer)++;  
    **buffer = MakeModrm (MODRM_MOD_RM, reg2, reg1); (*buffer)++;
}

void GenCmp (char** buffer, Reg_t reg1, Reg_t reg2)
{
    assert (buffer);
    assert (*buffer);
    
    **buffer = PREFIX_REX;                           (*buffer)++; 
    **buffer = OPCODE_CMP_R_WITH_RM;                 (*buffer)++; 
    **buffer = MakeModrm (MODRM_MOD_RM, reg2, reg1); (*buffer)++;
}

// const char OPCODE_JMP_NEAR      = 0xe9;
char MakeModrm (Modrm_mod_t mod, Reg_t reg, Modrm_rm_t rm)
{
    char modrm = 0;
    modrm |= mod << 6;  // xx *** *** mod 
    modrm |= reg << 3;  // ** xxx *** reg  
    modrm |= rm  << 0;  // ** *** xxx rm  

    return modrm;
}

void GenValue (char** buffer, int value)
{
    assert (buffer);
    assert (*buffer);
    
    memcpy (*buffer, &value, sizeof (int));
    *buffer += sizeof (int);
}

void GenDisp (char** buffer, int disp)
{
    assert (buffer);
    assert (*buffer);
    
    *(*buffer) = ((char*)&disp)[0]; (*buffer)++; 
}


int MySqrt (int x)
{
    return (int)(sqrt((double)x));
}
