#include<iostream>
using std::cout;
using std::endl;

struct Book {
    void printName () { cout << "common book" << endl; }
    virtual void speak () { cout << "Base" << endl; }
    Book () { speak (); }
};

struct TechBook : public Book {
   void printName () { cout<< "technique book" << endl;} // overwrite (distinc with overload)
   void speak () { cout << "subClass" << endl; }   // override
};

int main () {
    Book* b = new TechBook;
    b->printName ();
    b->speak ();
}
/// result:
// Base
// common book
// subClass
