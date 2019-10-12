//
// Created by chaos on 6/20/19.
//

#include <iostream>
#include <typeinfo>

using std::cout;
using std::endl;

class Base {
public:
    virtual void fun1() = 0;
    virtual void fun1(int i) { cout << "Base fun1: " << i << endl;};
    virtual void fun2(){ cout << "Base fun2: " << x << endl;};

    void fun3();
    void fun3(double x){ cout << "Base fun3: " << x << endl;};

private:
    int x;
};

class Derived: public Base {
public:
    using Base::fun1;

    virtual void fun1();
    void fun3();

    void fun3(double x) { Base::fun3(x);} // forwarding function
};

void Derived::fun1() { cout << typeid(this).name() << endl; }
void Derived::fun3() { cout << "Derived fun3" << endl; };

int main() {
   Derived d;
   int x = 1;

   d.fun1();
   d.fun1(x);

   d.fun2();

   d.fun3();
   d.fun3(0.0);

}