#include "r_instructions.h"
#include "cpu.h"

#include <stdio.h>
#include <string.h>


#define RSMACRO(instruction) (((*((unsigned int *)&instruction))&0x03E00000)>>21)

#define RTMACRO(instruction) (((*((unsigned int *)&instruction))&0x001F0000)>>16)

#define RDMACRO(instruction) (((*((unsigned int *)&instruction))&0x0000F800)>>11)

#define shamt(instruction) (((*((unsigned int *)&instruction))&0x000007C0)>>6)

#define funct(instruction) ((*((unsigned int *)&instruction))&0x0000003F)

//R[rd] ← R[rs] + R[rt]
void add(unsigned int instruction){
    unsigned int rs = (RSMACRO(instruction));
    unsigned int rt = (RTMACRO(instruction));
    unsigned int rd = (RDMACRO(instruction));
    int addR = (int) (registers[rs])  + (int) (registers[rt]);
    
    registers[rd] = (unsigned int) addR;

};

//R[rd] ← R[rs] + R[rt]
void addu(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = registers[rt] + registers[rs]; 
}; 

//R[rd] ← R[rs] - R[rt]
void sub(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    int subR = (int) (registers[rs]) - (int) (registers[rt]);
    registers[rd] = (unsigned int) subR; 

}; 

//R[rd] ← R[rs] - R[rt]
void subu(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = registers[rs] - registers[rt];
}; 

// HI:LO ← R[rs]*R[rt]
void mult(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    long multR = (long) registers[rs] * (long) registers[rt];
    HI = (multR & 0xFFFFFFFF00000000);
    LO = (multR & 0x00000000FFFFFFFF);

}; 

// HI:LO ← R[rs]*R[rt]
void multu(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned long multuR = (unsigned long) registers[rs] * (unsigned long) registers[rt];
    HI = (multuR &0xFFFFFFFF00000000) >> 32;
    LO = (multuR &0x00000000FFFFFFFF);
    
}; 

// LO ← R[rs] / R[rt]
// HI ← R[rs] % R[rt]
void div(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    int divLO = (int) (registers[rs]) / (int) (registers[rt]);
    int divHI = (int) (registers[rs]) % (int) (registers[rt]);
    LO = (unsigned int) divLO;
    HI = (unsigned int) divHI;
}; 

// LO ← R[rs] / R[rt]
// HI ← R[rs] % R[rt]
void divu(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    LO = registers[rs] / registers[rt];
    HI = registers[rs] % registers[rt];
}; 

//R[rd] ← R[rs] & R[rt]
void and(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    unsigned int andR = registers[rs] & registers[rt];
    registers[rd] = andR;
}; 

//R[rd] ← ~(R[rs] | R[rt])
void nor(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = ~(registers[rs] | registers[rt]);
}; 

//R[rd] ← R[rs] | R[rt]
void or(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = (registers[rs] | registers[rt]);
}; 

//R[rd] ← R[rs] ^ R[rt]
void xor(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = (registers[rs] ^ registers[rt]);
}; 

//R[rd] ← R[rt] << shamt
void sll(unsigned int instruction){
    unsigned int shamt = shamt(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = (registers[rt] << shamt);
}; 

//R[rd] ← R[rs] << R[rt]
void sllv(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = (registers[rs] << registers[rt]);
}; 

//R[rd] ← R[rt] >> shamt
void srl(unsigned int instruction){
    unsigned int shamt = shamt(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = (registers[rt] >> shamt);
}; 

//R[rd] ← R[rt] >> shamt
void sra(unsigned int instruction){
    unsigned int shamt = shamt(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
     unsigned int mask = 0x80000000;
    if(registers[rt] & mask)
    {
      unsigned int hold = registers[rt] >> shamt;
      mask = (1<<shamt) - 1;
      mask = mask << (32 - shamt);
      registers[rd] = hold|mask;
    }
    else
    {
      registers[rd] = registers[rt] >> shamt;
    }
}; 

//R[rd] ← R[rs] >> R[rt]
void srlv(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = registers[rs] >> registers[rt];
}; 

//R[rd] ← R[rs] >> R[rt]
void srav(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    unsigned int mask = 0x80000000;
    if(registers[rs] & mask){
      unsigned int hold = registers[rs] >> registers[rt];
      mask = (1 << registers[rt]) - 1;
      mask = mask << (32 - registers[rt]);
      registers[rd] = hold|mask;
    }
    else
    {
      registers[rd] = registers[rs] >> registers[rt];
    }
}; 

//If (R[rs] < R[rt])
// R[rd] ← 1
//else
// R[rd] ← 0
void slt(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    if( (int) registers[rs] < (int) registers[rt]){
        registers[rd] = 1;    
    }
    else{
        registers[rd] = 0;
    }
}; 

//If (R[rs] < R[rt])
// R[rd] ← 1
//else
// R[rd] ← 0
void sltu(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    unsigned int rt = RTMACRO(instruction);
    unsigned int rd = RDMACRO(instruction);
    int result = 0;
    if(registers[rs] < registers[rt]){
        result =  1;
        registers[rd] =  (unsigned int) result;
    }
    else{
        registers[rd] = (unsigned int) result;
    }
}; 

// npc ← R[rs]
void jr(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    npc = registers[rs];
}; 

// r[31] ← pc + 4
// npc ← r[rs] 
void jalr(unsigned int instruction){
    unsigned int rs = RSMACRO(instruction);
    registers[31] = pc + 4;
    npc = registers[rs];
}; 

//R[rd] ← HI
void mfhi(unsigned int instruction){
    unsigned int rd = RDMACRO(instruction);
    registers[rd] =  HI;
}; 

//R[rd] ← LO
void mflo(unsigned int instruction){
    unsigned int rd = RDMACRO(instruction);
    registers[rd] = LO;
}; 

/*
void syscall(unsigned int instruction);  //This is actually defined in syscall.c
*/
