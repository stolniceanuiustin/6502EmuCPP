#include "cpu_header.h"

// Here will be all the helper functions for emulating the instructions

void CPU::set_ZN(byte value)
{
    Z = (value == 0) ? 1 : 0;
    N = (value & 0x80) ? 1 : 0; //Checks bit 7 if it's set or not
}

// GROUP 1 INSTRUCTIOS
void CPU::ORA(uint16_t address, bool page_cross)
{
    // OR between accumulator and the contents at the given address
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    A = A | ram[address];
    set_ZN(A);

    cycles += lookup[inst.bbb] + (size_t)page_cross;
}

void CPU::AND(uint16_t address, bool page_cross)
{
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    A = A & ram[address];
    set_ZN(A);

    cycles += lookup[inst.bbb] + (size_t)page_cross;
}

void CPU::EOR(uint16_t address, bool page_cross)
{
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    A = A ^ ram[address];
    set_ZN(A);

    cycles += lookup[inst.bbb] + (size_t)page_cross;
}

void CPU::ADC(uint16_t address, bool page_cross)
{
    // TODO: TEST THIS
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    byte operand = ram[address];
    uint16_t result = (uint16_t)A + (uint16_t)(operand) + (uint16_t)(C);     //Accumulator + address + carry
    //bool overflow_check = (~(A ^ operand) & (A ^ result) & 0x80) != 0;
    //bool overflow_check = (A ^ result) & (operand ^ result) & 0x80 != 0;
    bool overflow_check = (A ^ result) & (operand ^ result) & 0x80;
    C = result > 0x00FF ? 1 : 0;    //if more than 1 byte => carry
    O = overflow_check ? 1 : 0;
    A = (byte)(result & 0x00FF);
    set_ZN(A);

    cycles += lookup[inst.bbb] + (size_t)page_cross;
}

void CPU::STA(uint16_t address)
{
    // Can't use IMEDIATE ADDRESSING
    // Stores the contents of the accumulator in memory
    // Doesnt change flags
    const int lookup[] = {6, 3, -1, 4, 6, 4, 5, 5};
    ram[address] = A;
    cycles += lookup[inst.bbb];
}

void CPU::LDA(uint16_t address, bool page_cross)
{
    //Loads A from memory
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    A = ram[address];
    set_ZN(A);
    cycles += lookup[inst.bbb] + (size_t)page_cross;
}

void CPU::CMP(uint16_t address, bool page_cross)
{
    //TODO not sure if the flags are set correctly but i think they are, will find out in unit testing
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};

    uint16_t result = A - ram[address];
    //cpu->SR &= ~(CARRY | NEGATIVE | ZERO);
    C = (result  >= 0x00FF) ? 1 : 0;
    set_ZN(result);
    cycles += lookup[inst.bbb] + (size_t)page_cross;
}

void CPU::SBC(uint16_t address, bool page_cross)
{
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    uint16_t result = A - ram[address] - !C; //Accumulator - memory contents - !carry 
    C = 0;
    set_ZN(A);

    // if (!(result & 0xFF00))
    //     C = 1;
    C = (result <= 0xFF) ? 1 : 0;
    bool overflow_check = (A ^ result) & (~(ram[address]) ^ result) & (1 << 7);
    O = overflow_check ? 1 : 0;

    A = (byte)(result & 0x00FF);
    cycles += lookup[inst.bbb] + (size_t)page_cross;
}
