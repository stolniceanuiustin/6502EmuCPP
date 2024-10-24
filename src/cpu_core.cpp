#include "cpu_header.h"
#include <iostream>
using std::cout;


const uint16_t null_address = 0;
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

uint16_t CPU::read_address_from_pc()
{
    uint16_t address = read_abs_address(PC);
    PC += 2;
    return address;
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

void CPU::run_instruction_group1(uint16_t address, bool page_cross)
{
    //TODO: implement all the functions and implement unit testing: CATCH2 (maybe)
    switch (inst.aaa)
    {
    case 0x0:
        ORA(address, page_cross);
        break;
    case 0x1:
        AND(address, page_cross);
        break;
    case 0x2:
        EOR(address, page_cross);
        break;
    case 0x3:
        ADC(address, page_cross);
        break;
    case 0x4:
        STA(address);
        break;
    case 0x5:
        LDA(address, page_cross);
        break;
    case 0x6:
        CMP(address, page_cross);
        break;
    case 0x7:
        SBC(address, page_cross);
        break;
    }
}

void CPU::run_instruction_group2(uint16_t address, bool page_cross, bool accumulator)
{
    switch (inst.aaa)
    {
    case 0x0:
        ASL(address, accumulator);
        break;
    case 0x1:
        ROL(address, accumulator);
        break;
    case 0x2:
        LSR(address, accumulator);
        break;
    case 0x3:
        ROR(address, accumulator);
        break;
    case 0x4:
        STX(address);
        break;
    case 0x5:
        LDX(address, page_cross);
        break;
    case 0x6:
        DEC(address);
        break;
    case 0x7:
        INC(address);
        break;
    }
}

void CPU::run_instruction_group3(uint16_t address, bool page_cross)
{
    uint16_t jump_address = 0;
    switch (inst.aaa)
    {
    case 0x0:
        printf("INVALID OPCODE \n");
        break;
    case 0x1:
        BIT(address);
        break;
    case 0x2:
        jump_address = read_address_from_pc();
        JMP_abs(jump_address);
        break;
    case 0x3:
        jump_address = read_address_from_pc();
        JMP_indirect(jump_address);
        break;
    case 0x4:
        // STY
        break;
    case 0x5:
        // LDY
        break;
    case 0x6:
        // CPY
        break;
    case 0x7:
        // CPX
        break;
    }
}

int CPU::execute()
{
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
        cout << "Opcode shouldn't be 0";
        // printf("%d : Opcode shouldn't be 0\n", cpu->PC);
        return 1;
    }

    PC += 1;

    inst.aaa = (0xE0 & inst.opcode) >> 5; // first 3 bits of the opcode
    inst.bbb = (0x1C & inst.opcode) >> 2; //second 3 bits 
    inst.cc = (0x03 & inst.opcode);       //last 2 bits 
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
        run_instruction_group1(address, page_cross);
        break;
    case 0x02: // cc = 10
        //Will return address via pointer, the function returns a boolean.
        onaddress_group2 = compute_addr_mode_g23(page_cross, address);
        if (onaddress_group2 == true)
            run_instruction_group2(address, page_cross, 0); // Not accumulator, on address
        else
            run_instruction_group2(null_address, page_cross, 1); // On accumulator
        break;
    case 0x0: // cc = 00
        compute_addr_mode_g23(page_cross, address);
        run_instruction_group3(address, page_cross);
        break;
    }

    //TODO: check if this is the correct way to call tracer
    TRACER my_tracer(*this);
    my_tracer.tracer(offset_address, page_cross, original_pc, onaddress_group2);
    (void)original_pc;
    return 1;
}