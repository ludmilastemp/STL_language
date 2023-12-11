/* MAKE_COND_JMP (name, opCode, comparison sign) */

/// номер команды не должен превышать 0x20 == 64

MAKE_COND_JMP (JAE, 0x1B, >= )

MAKE_COND_JMP (JA,  0x1A, >  )

MAKE_COND_JMP (JBE, 0x1D, <= )

MAKE_COND_JMP (JB,  0x1C, <  )

MAKE_COND_JMP (JE,  0x1E, == )

MAKE_COND_JMP (JNE, 0x1F, != )
