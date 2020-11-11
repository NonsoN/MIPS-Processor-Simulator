#include <stdio.h>

#include "i_instructions.h"
#include "cpu.h"

//These are the R-instructions implemented by the simulted MIPS processor
#define RSMACRO(x) ((x>>21)&0x1F)

#define RTMACRO(x) ((x>>16)&0x1F)

#define IMMEDIATE(x) ((x)&0xFFFF)

//These are the i-instructions implemented by the simulted MIPS processor
//sign extension for different bits - 16 to 32, 8 to 32

//R[rt] <-- R[rs] + sign_extend(imm)
void addi(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int immediate = IMMEDIATE(instruction);
    unsigned int mask = 0x8000;
    unsigned int mask2 = 0xFFFF0000;
    if(immediate & mask){
        immediate =  immediate | mask2;
    } 

    registers[rt] = (registers[rs] + immediate);
}

//R[rt] ← R[rs] + sign_extend(imm)
void addiu(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int immediate = IMMEDIATE(instruction);
    unsigned int mask = 0x8000;
    unsigned int mask2 = 0xFFFF0000;
    if(immediate & mask){
        immediate = immediate | mask2;
    }

    registers[rt] = registers[rs] + immediate;
}

//R[rt]←R[rs] & Imm
void andi(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  registers[rt] =  registers[rs] & immediate;
}

//R[rt]←R[rs] | Imm
void ori(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  registers[rt] =  registers[rs] | immediate;
}

//R[rt]←R[rs] ^ Imm
void xori(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  registers[rt] =  registers[rs] ^ immediate;
}

// if (R[rs] < sign_extend(imm))
// R[rt] ← 1 else R[rt] ← 0
void slti(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  unsigned int mask = immediate & 0xFFFF;
  if (immediate >> 15){
    mask = 0xFFFF0000 | immediate;
  }
  if( (int) registers[rs] < (int) mask){
    registers[rt] = 1;
  }
  else{
    registers[rt] = 0;
  }
}

// if (R[rs] < sign_extend(imm))
// R[rt] ← 1 else R[rt] ← 0
void sltiu(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  unsigned int mask = immediate & 0xFFFF;
  if (immediate >> 15){
    mask = 0xFFFF0000 | immediate;
  }
  if(registers[rs] < mask){
    registers[rt] = 1;
  }
  else{
    registers[rt] = 0;
  }
}

// if(R[rs] == R[rt])
// npc ← pc + 4 + (sign_extend(Immediate) << 2)
void beq(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int immediate = IMMEDIATE(instruction);
    unsigned int mask = 0x8000;
    unsigned int mask2 = 0xFFFF0000;
    if(immediate & mask){
        immediate = immediate | mask2;
    }

    if(registers[rs] == registers[rt]){
        npc = pc + 4 + (immediate << 2);
    } 
}

//if(R[rs] != R[rt])
// npc ← pc + 4 + (sign_extend(Immediate) << 2)
void bne(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int immediate = IMMEDIATE(instruction);
    unsigned int mask = 0x8000;
    unsigned int mask2 = 0xFFFF0000;
    if(immediate & mask){
        immediate = immediate | mask2;
    }

    if(registers[rs] != registers[rt]){
        npc = pc + 4 + (immediate << 2);
    }
}

//R[rt] ← sign_extend(M[R[rs]+sign_extend(Imm)])
void lb(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  unsigned int mask = immediate & 0xFFFF;
  if (immediate >> 15){
    mask = 0xFFFF0000 | immediate;
  }
  unsigned char hold = *((unsigned char *) & memory[registers[rs] + mask]);
  unsigned int temp = hold & 0xFF;
  if (hold >> 7){
    temp = 0xFFFFFF00 | hold;
  }
  int lbR = (int) temp;
  registers[rt] = lbR;
}

// R[rt] ← M[R[rs] + sign_extend(Imm)]
void lbu(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  unsigned int mask = immediate & 0xFFFF;
  if (immediate >> 15){
    mask = 0xFFFF0000 | immediate;
  }
  unsigned char hold = *((unsigned char *) & memory[registers[rs] + mask]);
  unsigned int lbuR = hold & 0xFF;
  registers[rt] = lbuR;
}

//R[rt] ← M[R[rs] + sign_extend(Imm)]
void lhu(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int immediate = IMMEDIATE(instruction);
    registers[rt] = ((registers[rt] >> 16) & 0xFF);
    unsigned int mask = 0x8000;
    unsigned int mask2 = 0xFFFF0000;
    if(immediate & mask){
        immediate = immediate | mask2;
    }
    unsigned short* x = (unsigned short*) &memory[registers[rs] + immediate];
    registers[rt] = *x;
}

//R[rt] ← sign_extend(M[R[rs]+sign_extend(Imm)])
void lh(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int immediate = IMMEDIATE(instruction);
    unsigned int y;
    unsigned int mask = 0x8000;
    unsigned int mask2 = 0xFFFF0000;
    if(immediate & mask){
        immediate = immediate | mask2;
    }
    unsigned short* x = (unsigned short*) &memory[registers[rs] + immediate];
    y = *x;
    if(y & mask){
        y = y | mask2;
        registers[rt] =  y;
    }
    else{
        registers[rt] = y;
    }
}

//R[rt]← M[R[rs]+ sign_extend(imm)]
void lw(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  unsigned int mask = immediate & 0xFFFF;
  if (immediate >> 15){
    mask = 0xFFFF0000 | immediate;
  }
  unsigned int lwResult = *((unsigned int *) & memory[registers[rs] + mask]);
  registers[rt] = lwResult;
}

//R[rt] ← (Imm << 16)
void lui(unsigned int instruction){
    unsigned int rt = RTMACRO(instruction);
    unsigned int immediate = IMMEDIATE(instruction);
    registers[rt] = (immediate << 16);
}

//M[R[rs] + sign_extend(Imm)] ← R[rt]
void sb(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int immediate = IMMEDIATE(instruction);
    unsigned int mask = immediate & 0xFFFF;
    if (immediate >> 15){
        mask = 0xFFFF0000 | mask;
    }
    unsigned char* sbResult = (unsigned char*) &memory[registers[rs] + mask];
    *sbResult = (unsigned char) (0xFF & registers[rt]);
}



//M[R[rs] + sign_extend(Imm)] ← R[rt]
void sh(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  unsigned int mask = immediate & 0xFFFF;
  if (immediate >> 15){
    mask = 0xFFFF0000 | immediate;
  }
  unsigned short *shR = (unsigned short*) &memory[registers[rs] + mask];
  *shR = (unsigned short) (0xFFFF & registers[rt]);
}

// M[R[rs] + sign_extend(Imm)] ← R[rt]
void sw(unsigned int instruction){
  unsigned int rs = RSMACRO(instruction);
  unsigned int rt = RTMACRO(instruction);
  unsigned int immediate = IMMEDIATE(instruction);
  unsigned int mask = immediate & 0xFFFF;
  if (immediate >> 15){
    mask = 0xFFFF0000 | immediate;
  }
  unsigned int* swR = (unsigned int*) &memory[registers[rs] + mask];
  *swR = registers[rt];
}
