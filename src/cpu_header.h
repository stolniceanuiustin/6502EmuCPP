#ifndef cpu_header
#define cpu_header
#include <stdint.h>

typedef uint8_t byte;

struct Mem
{
	int x;
};

struct Instruction
{
	byte aaa;
	byte bbb;
	byte cc;
	byte opcode;
};

class CPU
{
private:
	byte A; // Registrii Accumulator, X, Y
	byte X;
	byte Y;
	uint16_t PC;
	byte SP; // Stack pointer
	byte ram[65536];
	size_t cycles;

	// Cpu Flags
	byte C : 1; // carry
	byte Z : 1; // zero
	byte I : 1; // interrupt
	byte D : 1; // decimal
	byte B : 1; // break
	byte O : 1; // overflow
	byte N : 1; // negative

	struct Instruction inst;

public:
	uint16_t get_pc()
	{
		return PC;
	}
	uint16_t get_inst_opcode()
	{
		return inst.opcode;
	}
	bool reset();
	byte read_pc()
	{
		byte val = ram[PC];
		PC++;
		return val;
	}
	uint16_t read_address_from_pc();
	void write_byte(byte *address, byte value);
	uint16_t read_address(byte offset);
	int execute();
	uint16_t read_abs_address(uint16_t offset);

	bool compute_addr_mode_g23(bool &page_cross, uint16_t &address_to_return);

	// First group of instructions
	uint16_t compute_addr_mode_g1(bool &page_cross);
	void run_instruction_group1(uint16_t address, bool page_cross);
	void ORA(uint16_t address, bool page_cross);
	void AND(uint16_t address, bool page_cross);
	void EOR(uint16_t address, bool page_cross);
	void ADC(uint16_t address, bool page_cross);
	void STA(uint16_t address);
	void LDA(uint16_t address, bool page_cross);
	void CMP(uint16_t address, bool page_cross);
	void SBC(uint16_t address, bool page_cross);

	// Second group of instructions

	void run_instruction_group2(uint16_t address, bool page_cross, bool accumulator);
	void ASL(uint16_t address, bool accumulator);
	void ROL(uint16_t address, bool accumulator);
	void LSR(uint16_t address, bool accumulator);
	void ROR(uint16_t address, bool accumulator);
	void STX(uint16_t address);
	void LDX(uint16_t address, bool page_cross);
	void DEC(uint16_t address);
	void INC(uint16_t address);

	// Third group of instructions
	void CPU::run_instruction_group3(uint16_t address, bool page_cross);
	void JMP_abs(uint16_t jump_address);
	void JMP_indirect(uint16_t jump_address);
	void BIT(uint16_t address);
};

class TRACER
{
private:
	CPU& cpu;
public:
	TRACER(CPU& cpu_pointer) : cpu(cpu_pointer)
	{
	}
	void tracer(uint16_t address, bool page_cross, uint16_t original_pc, bool onaddress_group2);
};

#endif