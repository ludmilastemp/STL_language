/* MAKE_COND_JMP (name, opCode, comparison sign) */

/// ����� ������� �� ������ ��������� 0x20 == 64

MAKE_COND_JMP (jae, 0x1B, >= )

MAKE_COND_JMP (ja,  0x1A, >  )

MAKE_COND_JMP (jbe, 0x1D, <= )

MAKE_COND_JMP (jb,  0x1C, <  )

MAKE_COND_JMP (je,  0x1E, == )

MAKE_COND_JMP (jne, 0x1F, != )
