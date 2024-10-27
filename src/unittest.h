#include <stdint.h>
#include <iostream>
#include <bitset>

using std::cout;
using std::bitset;
class Assert
{
private:
    int test_count;

public:
    Assert()
    {
        test_count = 1;
    }

    void unit_test();
    void are_equal(uint8_t x, uint8_t y)
    {
        if (x == y)
        {
            cout << "Test " << test_count << " passed";
        }
        else
            cout << bitset<8>(x) << " != " << bitset<8>(y) << "." << " Test " << test_count << " failed";
        
        cout << '\n';
        test_count++;
    }

    void are_equal_flags(uint8_t x, uint8_t y, uint8_t o, uint8_t c, CPU& cpu)
    {
        if(x == y)
        {
            if(cpu.O == o && cpu.C == c)
            {
                cout << "Test " << test_count << " passed";
            }
            else if(cpu.O != o)
            {
                cout << "Test " << test_count << " failed because overflow " << bitset<1>(cpu.O) << " != " << bitset<1>(o);
            }
            else if(cpu.C != c)
            {
                cout << "Test " << test_count << " failed because overflow " << bitset<1>(cpu.C) << " != " << bitset<1>(c);
            }
        }
        else 
            cout << "Test " << test_count << " failed because " << bitset<8>(x) << " != " << bitset<8>(y);

        cout << '\n';
        test_count++;
    }

    void reset()
    {
        test_count = 1;
    }
};