#include <iostream>
#include "cpu_header.h"
#include "unittest.h"
using std::cout;

int main()
{
    CPU cpu;
    cpu.reset();
    
    Assert assert;
    assert.unit_test();
}