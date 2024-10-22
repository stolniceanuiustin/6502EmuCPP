#include "cpu_header.h"
#include <iostream>
using std::cout;

// byte CPU::read_byte(byte* address)
// {
//     // sleep(CLOCK_TIME);
//     return (*address);
// }

void CPU::write_byte(byte* address, byte value)
{
    // sleep(CLOCK_TIME);
    // sleep(0.1);
    *address = value;
    return;
}

uint16_t CPU::read_address(byte offset)
{
    uint16_t val = ram[offset+1]; // little endian
    val <<= 8;
    val |= ram[offset];
    return val;
}

//The difference between read_address and read_abs_address is that read_abs_address takes a 16bit offset
uint16_t CPU::read_abs_address(uint16_t offset)
{
    uint16_t val = ram[offset+1]; // little endian
    val <<= 8;
    val |= ram[offset];
    return val;
}
// i dont know about this fucntion, might come up later?
// uint16_t CPU::read_abs_address(byte* ram, uint16_t offset)
// {
//     uint16_t val = read_byte(ram + offset + 1);
//     val <<= 8;
//     val |= read_byte(ram + offset);
//     return (val);
// }

byte CPU::read_pc()
{
    byte val = ram[PC];
    PC++;
    return val;
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

int CPU::execute()
{
    // printf("%d\n", cpu->PC);
    // //SLEEP CODE(have no idea)
    // struct timespec ts;
    // ts.tv_sec = 0;            // Seconds
    // ts.tv_nsec = 500000000L;  // Nanoseconds (0.5 seconds)

    // nanosleep(&ts, NULL);
    
    
    bool onaddress_group2 = false;
    uint16_t offset_address = 0;
    uint16_t original_pc = PC;
    bool page_cross = false;

    if (original_pc == 0xFFFF)
    {
        cout << "End of program";
        //cpu->state = QUIT;
        return -1;
    }
    inst.opcode = read_pc();
    if (inst.opcode == 0)
    {
        // printf("%d : Opcode shouldn't be 0\n", cpu->PC);
        return 1;
    }

    PC += 1;

    inst.aaa = (0xE0 & inst.opcode) >> 5; // first 3 bits of the opcode
    inst.bbb = (0x1C & inst.opcode) >> 2;
    inst.cc = (0x03 & inst.opcode);
    byte low_nibble = inst.opcode & 0x0F;
    byte high_nibble = inst.opcode >> 4;
    uint16_t address = 0; //Offset of ram. no longer doing pointer arithmetic
    if (low_nibble == 0x08)
    {
        // Single byte instruction type 1
        // run_insturction_sb1(cpu);
    }
    if (low_nibble == 0x0A && high_nibble >= 0x08)
    {
        // Single byte instruction type 2
        // run_instruction_sb2(cpu);
    }
    switch (inst.cc)
    {
    case 0x01: // cc = 1
        address = compute_addr_mode_g1(page_cross);
        run_instruction_group1(address, cpu, page_cross);
        break;
    case 0x02: // cc = 10
        //Will return address via pointer, the function returns a boolean.
        onaddress_group2 = compute_addr_mode_g23(&page_cross, &address);
        if (onaddress_group2 == true)
            run_instruction_group2(address, cpu, page_cross, 0); // Not accumulator, on address
        else
            run_instruction_group2(NULL, cpu, page_cross, 1); // On accumulator
        break;
    case 0x0: // cc = 00
        compute_addr_mode_g23(cpu, &page_cross, &offset_address, &address);
        run_instruction_group3(address, cpu, page_cross, offset_address);
        break;
    }

    tracer(cpu, offset_address, page_cross, original_pc, onaddress_group2);
    (void)original_pc;
    return 1;
}