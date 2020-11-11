#include "cpu.h"
#include "r_instructions.h"
#include "i_instructions.h"
#include "j_instructions.h"
#include <stdio.h>
#include <string.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

#define OPMACRO(x) (x&0xFC000000)>>26
#define FUNCTMACRO(x) (x&0x0000003F)

#define WORD_SIZE_IN_BITS 32

unsigned int registers[NUM_REGISTERS];

//pc holds the current value of the PC (program counter). 
//IMPORTANT: pc may be read by an instruction, but should not 
//be modified by an instruction.
unsigned int pc;  

//npc is the value that the PC will be assigned to once the current
//instruction is completed. A jump or branch instruction should modify
//the value of npc if the jump or branch is taken.  If an instruction
//does not assign a value to npc, it will be given the value of
//pc+4.  IMPORTANT: npc may be modified by an instruction (but only
//by jump or branch), but it should not be read by an instruction.
unsigned int npc;

//HI and LO are the registers assigned by the MULT and DIV instructions
//and read by the MFHI and MFLO instructions.
unsigned int HI;
unsigned int LO;

//Memory is an array of bytes (not words).
unsigned char memory[MEMORY_SIZE_IN_BYTES];

void handle_r_instruction(unsigned int instruction)
{
  switch(FUNCTMACRO(instruction)){
    case 0x20:
      add(instruction);
      break;
    case 0x21:
      addu(instruction);
      break;
    case 0x22:
      sub(instruction);
      break;
    case 0x23:
      subu(instruction);
      break;
    case 0x18:
      mult(instruction);
      break;
    case 0x19:
      multu(instruction);
      break;
    case 0x1A:
      div(instruction);
      break;
    case 0x1B:
      divu(instruction);
      break;
    case 0x10:
      mfhi(instruction);
      break;
    case 0x12:
      mflo(instruction);
      break;
    case 0x24:
      and(instruction);
      break;
    case 0x25:
      or(instruction);
      break;
    case 0x26:
      xor(instruction);
      break;
    case 0x27:
      nor(instruction);
      break;
    case 0x2A:
      slt(instruction);
      break;
    case 0x2B:
      sltu(instruction);
      break;
    case 0x00:
      sll(instruction);
      break;
    case 0x04:
      sllv(instruction);
      break;
    case 0x02:
      srl(instruction);
      break;
    case 0x06:
      srlv(instruction);
      break;
    case 0x03:
      sra(instruction);
      break;
    case 0x07:
      srav(instruction);
      break;
    case 0x08:
      jr(instruction);
      break;
    case 0x09:
      jalr(instruction);
      break;
    case 0x0C:
      syscall(instruction);
      break;  
  }
}


//This is the procedure for initializing (or reinitializing) the CPU.
void cpu_initialize()
{
  pc = CODE_STARTING_ADDRESS;
  npc = CODE_STARTING_ADDRESS;
  registers[29] = STACK_STARTING_ADDRESS;
  registers[0] = 0;
}

int flag;
//This is the procedure for causing the CPU to execute. It should
//start by executing the instruction at address CODE_STARTING_ADDRESS
void  cpu_execute()
{
  flag = 0;
  while(flag == 0){
    pc = npc;
    npc = npc + 4;
    unsigned int instruction = *((unsigned int*) &memory[pc]);
    switch(OPMACRO(instruction)){
      case 0:
        handle_r_instruction(instruction);
        break;
      case 0x08:
        addi(instruction);
        break;
      case 0x09:
        addiu(instruction);
        break;
      case 0x23:
        lw(instruction);
        break;
      case 0x21:
        lh(instruction);
        break;
      case 0x25:
        lhu(instruction);
        break;
      case 0x20:
        lb(instruction);
        break;
      case 0x24:
        lbu(instruction);
        break;
      case 0x2b:
        sw(instruction);
        break;
      case 0x29:
        sh(instruction);
        break;
      case 0x28:
        sb(instruction);
        break;
      case 0x0F:
        lui(instruction);
        break;
      case 0x0D:
        ori(instruction);
        break;
      case 0x0C:
        andi(instruction);
        break;
      case 0x0E:
        xori(instruction);
        break;
      case 0x0A:
        slti(instruction);
        break;
      case 0x0B:
        sltiu(instruction);
        break;
      case 0x04:
        beq(instruction);
        break;
      case 0x05:
        bne(instruction);
        break;
      case 0x02:
        j(instruction);
        break;
      case 0x03:
        jal(instruction);
        break;   
    }
  }
}

//This procedure will cause the CPU to stop executing the current program. 
//If errorcode is 0, a message indicating normal termination will be printed. 
//Otherwise, a message indicating abnormal termination, along with the
//errorcode, will be printed.
void cpu_exit(int errorcode)
{
  flag = 1;
  if(errorcode == 0)
    printf("Program terminated normally\n");
  else
    printf("Program terminated with error %d\n",errorcode);
}
