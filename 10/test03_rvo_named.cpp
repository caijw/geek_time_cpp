#include <iostream>  // std::cout

using namespace std;

// Can copy and move
class A {
public:
    A() { cout << "Create A\n"; }
    ~A() { cout << "Destroy A\n"; }
    A(const A&) { cout << "Copy A\n"; }
    A(A&&) { cout << "Move A\n"; }
};

A getA_named()
{
    A a;
    return a;
}

A getA_unnamed()
{
    return A();
}

int main()
{   
    {
        std::cout << "getA_named\n";
        auto a = getA_named();
    }
    {
        std::cout << "getA_unnamed\n";
        auto b = getA_unnamed();
    }

}
