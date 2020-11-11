#include "j_instructions.h"
#include "cpu.h"

#include <stdio.h>
#include <string.h>

//These are the J-instructions implemented by the simulted MIPS processor

//J instruction:  opcode (6 bits)      address (26 bits)

// 1111 1100 0000 0000 0000 0000 0000 0000
#define OPCODE(x) ((x >> 26) & 0xCF)
// 0000 0011 1111 1111 1111 1111 1111 1111
#define ADDRESS(x) (x & 0x3FFFFF)

//npc ← ((pc+4) & 0xf0000000) | (address << 2)
void j(unsigned int instruction){
    unsigned int address = ADDRESS(instruction);
    npc = ((pc + 4) & 0xF0000000) | (address << 2);
}

//R[31] ← pc+4
// npc ← ((pc+4) & 0xf0000000) | (address << 2)
void jal(unsigned int instruction){
    unsigned int address = ADDRESS(instruction);
    registers[31] = pc+4;
    npc = ((pc+4) & 0xF0000000) | (address << 2);
}