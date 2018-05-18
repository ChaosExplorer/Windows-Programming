//
// Created by chaos on 5/16/18.
//

#ifndef SAMHANDLER_FILTER_H
#define SAMHANDLER_FILTER_H

#include "Repository.h"

struct FilterStandard {
    char xt;
    int nm;
    int mapQ;
};

class Filter{
    FilterStandard _cond;
public:
    Filter(FilterStandard& cond): _cond(cond) {}

    void goForAll(char *buffer, sortIndecVec &indecVec, StasticsData &stat);

    bool checkRecord(char* record, SamSortIndex& index, SamLine& line);
};


#endif //SAMHANDLER_FILTER_H
