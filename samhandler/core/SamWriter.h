//
// Created by chaos on 5/16/18.
//

#ifndef SAMHANDLER_SAMWRITER_H
#define SAMHANDLER_SAMWRITER_H

#include "Repository.h"
#include <string>
#include <fstream>
#include <iostream>
using std::string;

class Samwriter {
    string _outPath;
    std::ofstream _out;

public:
    Samwriter(const string& outputPath): _outPath(outputPath) //, _out(std::ofstream(outputPath))
    {
        _out.open(_outPath, std::ofstream::out);
    }

    ~Samwriter()
    {
       _out.close();
    }

    void writeAll(char *buffer, const sortIndecVec &indecVec, const off64_t header_endPos);

    void writePart (char* buffer, uint size);
};


#endif //SAMHANDLER_SAMWRITER_H
