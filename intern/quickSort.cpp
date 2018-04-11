#include <iostream>
#include <vector>
using std::cout;
using std::cin;
using std::endl;
using std::vector;

void quickSort (vector<int>& vec, vector<int>::iterator begin, vector<int>::iterator end)
{
    if (begin >= end)
	return;

    vector<int>::iterator head = begin, tail = end - 1;
    int temp = *tail;

    while (head < tail)
    {
	while (head < tail && *head <= temp)
	    ++head;
	*tail = *head;

	while (head < tail && *tail >= temp)
	   --tail;
	*head = *tail;
    }
    *tail = temp;

    // randon access iterator, distance: return  __last - __first 
    if (distance(begin, tail) > 1) 		
	quickSort (vec, begin, tail);
    
    if (distance (tail, end) > 2)  
	quickSort (vec, tail + 1, end);
}

int main ()
{
   vector<int> vec;
   int iTemp;

   while (cin >> iTemp)
	vec.push_back (iTemp);

   quickSort (vec,vec.begin(),vec.end());
   
   cout << endl;
   for (vector<int>::iterator it = vec.begin(); it != vec.end (); ++it)
   {
	cout << *it << endl;
   }

}
