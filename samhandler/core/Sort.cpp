//
// Created by chaos on 5/16/18.
//

#include "Sort.h"


void Sorter::quickSort(sortIndecVec &indexVec, sortIndecVec::iterator begin, sortIndecVec::iterator end)
{
    if (begin >= end)
        return;

    sortIndecVec::iterator head = begin, tail = end - 1;
    SamSortIndex temp = *tail;

    while (head < tail)
    {
        while (head < tail && *head <= temp)
            ++head;
        *tail = *head;

        while (head < tail && temp <= *tail)
            --tail;
        *head = *tail;
    }
    *tail = temp;

    // randon access iterator, distance: return  __last - __first
    if (distance(begin, tail) > 1)
        quickSort (indexVec, begin, tail);

    if (distance (tail, end) > 2)
        quickSort (indexVec, tail + 1, end);

}

void Sorter::mergeSort(sortIndecVec &indecVec) {

}
