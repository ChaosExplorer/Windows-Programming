//
// Created by chaos on 6/21/19.
//

#include <iostream>
#include <typeinfo>

class Airport {
private:
    int id;
};

class Airplane {
public:
    Airplane(unsigned int x):id(x) {}
    virtual ~Airplane() {};
    virtual void declIdentity () = 0;
    virtual void fly (const Airport& destination);

private:
    unsigned int id;
};

void Airplane::declIdentity() {std::cout << id << std::endl;}
void Airplane::fly(const Airport &destination) { std::cout << typeid(*this).name() <<std::endl;}

class ModelA:public Airplane {
public:
    ModelA (unsigned int x): Airplane(x) {}
    virtual void declIdentity () { Airplane::declIdentity(); }
};

class ModelB:public Airplane {
public:
    ModelB (unsigned int x): Airplane(x) {}
    virtual void declIdentity () { Airplane::declIdentity();}

    virtual void fly(const Airport &);
};

void ModelB::fly(const Airport &destination) {std::cout << "Model B flying" << std::endl;}

int main() {
    Airport SZX;
    Airplane* pa = new ModelA(1);
    Airplane* pb = new ModelB(2);

    pa->declIdentity();
    pa->fly(SZX);
    pb->declIdentity();
    pb->fly(SZX);

    delete pa;
    delete pb;

    return 0;
}