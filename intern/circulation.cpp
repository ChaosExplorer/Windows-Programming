#include <iostream>
using std::cout;
using std::endl;

int main () {
    for (int i = 5, j = -5; i = j = 0 /*i == j == 0*/ ; --i, ++j) 
    {
        cout << "i : " << i << ", j : " << j << endl;
    }

    int i = 5, j = -5;
    int d = i = 5;                   // d = 5 
    bool a = i = j = 0;
    bool b = j = 0;
    int c = i = j;
}
