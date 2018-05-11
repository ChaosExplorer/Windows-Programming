// FastqReader.cpp
// Author : chaos
// Date :   5/4/18.
//

#include <cstring>
#include <iostream>
#include <iomanip>
#include <thread>
#include <sstream>
#include "FastqHandler.h"

#define DEFAULT_OUTPUT_PATH "/result.gz"

using std::cout;
using std::endl;
using std::unique_lock;

FastqHandler::FastqHandler(const string &srcPath)
    : _srcPath(srcPath), _seqSum(0), _effectiveSum(0),
      _bFinishReading(false), _backOffset(0)
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

    _fetchProc = std::bind(&FastqHandler::gz_fetch_proc, this);
    _writeProc = std::bind(&FastqHandler::gz_write_proc, this);
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

    thread fetchThread(_fetchProc);
    //thread fetchThread([this]{ this->gz_fetch_proc();});
    //thread writeThread([this]{ this->gz_write_proc();});
    thread writeThread(_writeProc);

    if(fetchThread.joinable())
        fetchThread.join();
    if(writeThread.joinable())
        writeThread.join();

    sleep(10);
}

// judge the dnaSeq'effectiveness
bool FastqHandler::judgeTheSeq(dnaSeq &seq) {
    int seqNum = strlen(seq.quality);
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

void FastqHandler::gz_fetch_proc()
{
    try {
        int err, bytes_read_num;
        string buff;
        buff.reserve(BUFF_LENGTH);

        while (1) {
            //clean old data
            buff.clear();

            // backtrace _backOffset
            if (_backOffset)
                gzseek(_fastqFile, _backOffset, SEEK_CUR);

            bytes_read_num = gzread(_fastqFile, &buff, BUFF_LENGTH);

            buffAnalysis(buff);

            if (bytes_read_num < BUFF_LENGTH) {
                if (gzeof(_fastqFile)) {          // end of file
                    _bFinishReading = true;

                    // output the report
                    cout << "There are " << _seqSum << " DNASequence in the source." << endl;
                    cout << _effectiveSum << " effective." << endl;
                    cout << std::setprecision (8) << "The radio is " << (double)_effectiveSum/(double)_seqSum*100 << "%." << endl;
                    break;
                }
                else {                          // err happened
                   string errStr = gzerror (_fastqFile, &err);
                   if (err) {
                        cout << "error ocurred in gz_fetch_proc() :" << errStr << endl;
                   }
                   break;
                }
            }
        }
    }
    catch (...) {
        cout << "error ocurred in gz_fetch_proc()." << endl;
    }
}

void FastqHandler::gz_write_proc()
{
    try {
        int err, bytes_write_num;
        unsigned long buffSize = 0;
        unsigned char buff[BUFF_LENGTH];

        while (!_bFinishReading) {
            // copy buff from shared memory
            {
                unique_lock<mutex> lock(_repo.mtx);
                while (_repo.bFill) {
                    _repo.repo_not_empty.wait(lock);
                }

                buffSize = _repo.buffSize;
                memcpy(buff, _repo.buff, buffSize);      // key

                _repo.bFill = false;

                _repo.repo_not_full.notify_all();
            }

            bytes_write_num = gzwrite (_outFile, buff, buffSize);

            if (!bytes_write_num) {
                // err happened
                string errStr = gzerror (_fastqFile, &err);
                if (err) {
                    cout << "error ocurred in gz_write_proc() :" << errStr << endl;
                }
                break;
            }
        }


        if (!err)
            cout << "Here is the result : " << _outPath << endl;
    }
    catch (...) {
        cout << "error ocurred in gz_write_proc()." << endl;
    }
}

void FastqHandler::buffAnalysis(string &buff)
{
    size_t leftPos = 0, rightPos = 0;
    unsigned int index = 0;
    dnaSeq seq;
    unsigned long lastEffetivePos = 0;

    while ((rightPos = buff.find('\n', leftPos))!= string::npos) {
        if (++index % 4 == 0) {
            ++_seqSum;
            strncpy (seq.quality, buff.substr(leftPos, rightPos-1).c_str(), SEQLENGTH);
            if (judgeTheSeq (seq)) {
                ++_effectiveSum;
                lastEffetivePos = rightPos;
            }
            else {
                leftPos = lastEffetivePos + 1;
                buff.erase(leftPos, rightPos);
                continue;
            }
        }

        leftPos = rightPos + 1;
    }

    // part data fetched
    _backOffset = buff.size() - 1 - lastEffetivePos;

    // copy to shared memory
    {
        unique_lock<mutex> lock(_repo.mtx);
        while (!_repo.bFill)
            _repo.repo_not_full.wait(lock);

        _repo.buffSize = buff.size();
        memcpy (_repo.buff, &buff, _repo.buffSize);

        _repo.bFill = true;
        _repo.repo_not_empty.notify_all();
    }
}
