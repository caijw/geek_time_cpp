#include <bitset>    // std::bitset
#include <iostream>  // std::cout/endl

using namespace std;

int main()
{
    unsigned mask = 0b111000000;
    cout << bitset<9>(mask) << endl;
    {
        unsigned mask = 0b111'000'000;
        long r_earth_equatorial = 6'378'137;
        double pi = 3.14159'26535'89793;
        const unsigned magic = 0x44'42'47'4E;
    }
}
