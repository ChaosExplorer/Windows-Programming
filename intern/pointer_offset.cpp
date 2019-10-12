#include <iostream>
#include <iomanip>
using std::endl;
using std::cout;
using std::hex;
int main(int argc, char** argv)
{
  int* p = new int[10];

  for(int i=0; i<10; ++i)
  {
    p[i] = i;
  }

  cout << "&p : " << p << endl;

  cout << "p+5: " << *(p+5) << endl;

  cout << &p[5]<< endl;

  cout << p+1 << endl;
  cout << p+2 << endl;
  cout << p+3 << endl;
  cout << p+4 << endl;
  cout << p+5 << endl;

  cout << sizeof(int) << endl;
  cout << p+5*sizeof(int) << endl;

  long int pa = (long int)p;
  cout << hex << pa<<endl;
  cout << hex << pa+5*sizeof(int) << endl;
  delete p;
  
  void* vp = reinterpret_cast<void*>(p);
  cout << vp << endl;
  cout << vp+1 << endl;
}
