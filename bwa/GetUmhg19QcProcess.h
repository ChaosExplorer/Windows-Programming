//
// Created by Pason on 2019-7-1.
//

#include "GetUmhg19Qc.h"

#define LISTLINEBUFSIZE 1024
#define STATFILEBUFSIZE 1024
#define NNUMPER (0.06)
#define Q5PER (0.3)

using namespace std;

class GetUmhg19QcProcess{
    GetUmhg19QcProcess(float mapRate, int thdNum);
    ~GetUmhg19QcProcess();

public:
	void filterStat(const char * samfileName, const int fileFd, GetUmhg19Qc & getUmhg19Qc);
	int doWork(const int argc, char **argv );
	void initial();
	int outputStat(char* statFilePath);


private:
    vector<GetUmhg19Qc *> getUmhg19QcVec;
    unsigned int thread_num;
    float mapRate;
};
