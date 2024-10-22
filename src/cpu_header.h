#ifndef CPUTYPES
#define CPUTYPES
#include <stdint.h>

typedef uint8_t byte;

struct Mem {
	int x;
};

struct Instruction
{
	byte aaa;
	byte bbb;
	byte cc;
	byte opcode;
};

struct CPU
{
	byte A;		//Registrii Accumulator, X, Y
	byte X;
	byte Y;
	uint16_t PC;
	byte SP;	//Stack pointer
	byte ram[65536];
	size_t cycles;


	//Cpu Flags
	byte C : 1;		//carry
	byte Z : 1;		//zero
	byte I : 1;		//interrupt
	byte D : 1;		//decimal
	byte B : 1;		//break
	byte O : 1;		//overflow
	byte N : 1;		//negative 

	Instruction inst;
	

	bool reset();
	byte read_pc();
	void write_byte(byte* address, byte value);
	uint16_t read_address(byte offset);
	int execute();
	uint16_t read_abs_address(uint16_t offset);
	uint16_t compute_addr_mode_g1(bool& page_cross);
	bool compute_addr_mode_g23(bool &page_cross, byte &address_to_return);
};

#endif