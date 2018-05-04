// FastqReader.cpp
// Author : chaos
// Date :   5/4/18.
//

#include <cstring>
#include <iostream>
#include <iomanip>
#include "FastqHandler.h"

#define DEFAULT_OUTPUT_PATH "/result.gz"

using std::cout;
using std::endl;
//using std::FILE;

FastqHandler::FastqHandler(const string &srcPath)
    : _srcPath(srcPath)
{
    _outPath = getParrentPath(_srcPath) + DEFAULT_OUTPUT_PATH;
    _fastqFile = gzopen(srcPath.c_str(), "rb");
    _outFile = gzopen(_outPath.c_str(), "wb");

    if (_fastqFile == NULL) {
        fprintf(stderr, "gzopen source error \n");
        exit(1);
    }

    if (_outFile == NULL) {
        fprintf(stderr, "gzopen output file error \n");
        exit(1);
    }
}

FastqHandler::~FastqHandler()
{
    gzclose_r(_fastqFile);
    gzclose_w(_outFile);
}

// filter fastq data from given file path with the specified standard
void FastqHandler::filter(unsigned int standard_score, double standard_ratio)
{
    _filterStandard_score = standard_score;
    _filterStandard_ratios = standard_ratio;

    ulong count = 0, effectiveSum = 0;
    dnaSeq curSeq;

    while (gz_readSeq(curSeq)) {
        ++count;
        if (judgeTheSeq(curSeq)) {
            ++effectiveSum;
            gz_wrieSeq(curSeq);
        }
    }

    // output the report
    cout << "There are " << count << " DNASequence in the source." << endl;
    cout << effectiveSum << " effective." << endl;
    cout << std::setprecision (8) << "The radio is " << (double)effectiveSum/(double)count*100 << "%." << endl;
    cout << "Here is the result : " << _outPath << endl;
}

// read a line from gz file.
bool FastqHandler::gz_getline(string &str)
{
    char buff[MAX_LENGTH];
    if (gzgets(_fastqFile, buff, MAX_LENGTH) != NULL) {
        str = string(buff, strlen(buff));       // notice the newline character.
        return true;
    }
    return false;
}

// read a dnaSeq from the source gzfile
bool FastqHandler::gz_readSeq(dnaSeq &seq)
{
    string buffStr;
    int index = 0;
    try {
        while (++index <= 4) {
            if (!gz_getline(buffStr))
                return false;

            switch (index) {
                case 1:
                    strncpy(seq.id, buffStr.c_str(), buffStr.length());
                    break;
                case 2:
                    strncpy(seq.baseSeq, buffStr.c_str(), buffStr.length());
                    break;
                case 3:
                    break;
                case 4:
                    strncpy(seq.quality, buffStr.c_str(), buffStr.length());
                    break;
            }
        }

        return true;
    }
    catch (...) {
        cout << "error ocurred in gz_ReadSeq()." << endl;
    }


    return false;
}

// write a dnaSeq into the output gzfile
bool FastqHandler::gz_wrieSeq(dnaSeq &seq)
{
    try {
        gzputs(_outFile, seq.id);
        gzputs (_outFile, seq.baseSeq);
        gzputs(_outFile, "+\n");
        gzputs(_outFile, seq.quality);

        return true;
    }
    catch (...) {
        cout << "error ocurred in gz_wrieSeq()." << endl;
    }


    return false;
}

// judge the dnaSeq'effectiveness
bool FastqHandler::judgeTheSeq(dnaSeq &seq) {
    int seqNum = strlen(seq.quality)-1;
    if (seqNum <= 0)
        return false;

    int effectNum = 0, score;

    for (int i = 0; i < seqNum; ++i) {
        score = seq.quality[i]-33;
        if ( score > _filterStandard_score)
            ++effectNum;
    }

    return (double)effectNum/seqNum*100 > _filterStandard_ratios;
}

string FastqHandler::getParrentPath(const string &path) {
    size_t offset;
    offset = path.find_last_of('/');
    return path.substr(0, offset);
}
