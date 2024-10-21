#ifndef CPUTYPES
#define CPUTYPES
#include <stdint.h>

typedef uint8_t byte;

struct Mem {
	int x;
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


	bool reset();
	byte read_byte(byte* address);
	void write_byte(byte* address, byte value);
	uint16_t read_address(byte* ram, byte offset);
	int execute_cpu();
	uint16_t read_abs_address(byte* ram, uint16_t offset);

};

#endif