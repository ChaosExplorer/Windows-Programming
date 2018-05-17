//
// Created by chaos on 5/16/18.
//

#ifndef SAMHANDLER_SAMWRITER_H
#define SAMHANDLER_SAMWRITER_H

#include "Repository.h"
#include <string>
using std::string;

class Samwriter {
    string _outPath;
public:
    Samwriter(const string& outputPath): _outPath(outputPath){};

    void writeAll (char* buffer, sortIndecVec& indecVec);

    void writePart (char* buffer, uint size);
};


#endif //SAMHANDLER_SAMWRITER_H
