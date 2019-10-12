#include <iostream>
using std::cout;

int main(int argc, char** argv)
{
  const int i = 0;
  const int* p = &i;
  cout << *p;
  
  const int& q = i;
  int& u = const_cast<int&>(i);
  u = 2;
  cout << u;
  int *t = const_cast<int*>(&i);
  *t = 1;
  cout << *t;

  //int h = const_cast<int>(i);
}
