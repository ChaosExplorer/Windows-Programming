//
// Created by chaos on 5/16/18.
//

#ifndef SAMHANDLER_SORTER_H
#define SAMHANDLER_SORTER_H

#include "Repository.h"


class Sorter {

public:
    static void quickSort (sortIndecVec& indexVec, sortIndecVec::iterator begin, sortIndecVec::iterator end);
    static void mergeSort (sortIndecVec& indecVec);
};


#endif //SAMHANDLER_SORTER_H
