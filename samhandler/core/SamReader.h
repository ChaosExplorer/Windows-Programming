//
// Created by chaos on 5/15/18.
//

#ifndef SAMHANDLER_SAMREADER_H
#define SAMHANDLER_SAMREADER_H

#include <string>
#include <fstream>
using std::string;

class SamReader {
    string _filePath;
    std::ifstream _in;

public:
    SamReader(const string& filePath);
    ~SamReader();

    bool readAll(char *, unsigned long int);

    bool readPart(char *, unsigned long int);

    bool seekPos(int pos);
};


#endif //SAMHANDLER_SAMREADER_H
