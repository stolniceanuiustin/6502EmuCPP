#include "cpu_header.h"
#include "unittest.h"
#include <iostream>

using std::cout;

void test_ora(Assert& assert, CPU& cpu, byte operand1, byte operand2, byte expected_result)
{
    cpu.A = operand1;
    cpu.ram[0] = operand2;
    cpu.ORA(0, 0);
    assert.are_equal(expected_result, cpu.A);
}

void test_and(Assert& assert, CPU& cpu, byte operand1, byte operand2, byte expected_result)
{
    cpu.A = operand1;
    cpu.ram[0] = operand2;
    cpu.AND(0, 0);
    assert.are_equal(expected_result, cpu.A);
}

void test_adc(Assert& assert, CPU& cpu, byte operand1, byte operand2, byte expected_result, byte expected_overflow, byte expected_carry)
{
    cpu.reset();
    cpu.A = operand1;
    cpu.ram[0] = operand2;
    cpu.ADC(0, 0);
    assert.are_equal_flags(expected_result, cpu.A, expected_overflow, expected_carry, cpu);
}

void test_group_1(Assert& assert, CPU& cpu)
{
    cout << "ORA Tests\n";
    //Test 1
    test_ora(assert, cpu, 0b00000000, 0b00001111, 0b00001111);
    test_ora(assert, cpu, 0b00000000, 0b00001111, 0b00001111);
    test_ora(assert, cpu, 0b00000000, 0b00011111, 0b00011111);
    test_ora(assert, cpu, 0b00000000, 0b00111111, 0b00111111);
    
    assert.reset();
    cout << "AND Tests\n";
    test_and(assert, cpu, 0b00000000, 0b00001111, 0b00000000);
    test_and(assert, cpu, 0b00000000, 0b00001111, 0b00000000);
    test_and(assert, cpu, 0b00000000, 0b00011111, 0b00000000);
    test_and(assert, cpu, 0b00000000, 0b00111111, 0b00000000);


    //skipping over EOR
    
    assert.reset();
    cout << "ADC Tests\n";
    test_adc(assert, cpu, 0x50, 0x10, 0x60, 0, 0);
    test_adc(assert, cpu, 0x50, 0x50, 0xA0, 1, 0);
    test_adc(assert, cpu, 0x50, 0x90, 0xE0, 0, 0);
    test_adc(assert, cpu, 0x50, 0xd0, 0x20, 0, 1);
    test_adc(assert, cpu, 0xd0, 0x10, 0xe0, 0, 0);
    test_adc(assert, cpu, 0xd0, 0x50, 0x20, 0, 1);
    test_adc(assert, cpu, 0xd0, 0x90, 0x60, 1, 1);
    test_adc(assert, cpu, 0xd0, 0xd0, 0xa0, 0, 1);

	
    /*
	void ADC(uint16_t address, bool page_cross);
	void STA(uint16_t address);
	void LDA(uint16_t address, bool page_cross);
	void CMP(uint16_t address, bool page_cross);
	void SBC(uint16_t address, bool page_cross);
    */


}

void Assert::unit_test()
{
    CPU cpu;
    cpu.reset();
    test_group_1(*this, cpu);
}
