//
// Created by chaos on 5/15/18.
//
#include <iostream>
#include "SamReader.h"

SamReader::SamReader(const string &filePath)
    :_filePath (filePath), _in(std::ifstream(filePath))
{
    if (!_in) {
        std::cout << _filePath << " open error\n";
    }
}

SamReader::~SamReader()
{
    _in.close();
}

bool SamReader::readAll(char *buff, unsigned long size)
{
    _in.read (buff, size);
    return _in.eof();
}

bool SamReader::seekPos(int pos)
{
    _in.seekg(pos, _in.cur);
    return _in.eof() || _in.fail();
}