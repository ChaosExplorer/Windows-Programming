#include <iostream>
#include "FastqHandler.h"
using std::cout;
using std::endl;

int main(int argc, char** argv) {

    bool bFailed = false;
    unsigned int score;
    double ratio;

    try {
        if (argc > 3) {
            score = (unsigned int)atoi(argv[2]);
            ratio = atof(argv[3]);

            if (score <= 0 || score >= 41 || ratio <= 0 || ratio >100)
                bFailed = true;
        }
        else
            bFailed = true;

    }
    catch (...) {
        bFailed = true;
    }

    if (bFailed) {
        cout << "please check your input parameters\nthe format should be :\n   \
                proc [fastq path] [filter score] [filter ratio]" << endl;
        return 0;
    }

    FastqHandler fastqHandler (argv[1]);
    fastqHandler.filter(score, ratio);

    return 0;
}