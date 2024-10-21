#include "cpu_header.h"


byte CPU::read_byte(byte* address)
{
    // sleep(CLOCK_TIME);
    return (*address);
}

void CPU::write_byte(byte* address, byte value)
{
    // sleep(CLOCK_TIME);
    // sleep(0.1);
    *address = value;
    return;
}

uint16_t CPU::read_address(byte* ram, byte offset)
{
    uint16_t val = read_byte(ram + offset + 1); // little endian
    val <<= 8;
    val |= read_byte(ram + offset);
    return (val);
}
uint16_t CPU::read_abs_address(byte* ram, uint16_t offset)
{
    uint16_t val = read_byte(ram + offset + 1);
    val <<= 8;
    val |= read_byte(ram + offset);
    return (val);
}

bool CPU::reset()
{
    A = 0;
    X = 0;
    Y = 0;
    SP = 0;
    PC = 0;
    for (int i = 0; i < 0xFFFF; i++)
        ram[i] = 0;
    cycles = 0;
    return true;
}
