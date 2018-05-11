// FastqReader.h
// Author : chaos
// Date :   5/4/18. //

#ifndef FASTQ_GZ_HANDLER_FASTQREADER_H
#define FASTQ_GZ_HANDLER_FASTQREADER_H

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include "zconf.h"
#include "zlib.h"

using std::string;
using std::thread;
using std::mutex;
using std::condition_variable;
using std::atomic_bool;

#define SEQLENGTH 35
#define IDLENGTH 30
#define BUFF_LENGTH 10485760
#define MAX_LENGTH 128

// object struct to save the raw fastq data
typedef struct dnaSeq {
    char id[IDLENGTH];
    char baseSeq[SEQLENGTH];
    char quality[SEQLENGTH];
} *pDnaSeq;

typedef struct buffRespository {
    buffRespository():bFill(false){}
    char buff[BUFF_LENGTH];
    unsigned long buffSize;
    bool bFill;         // flag that the buff is filled with data
    mutex mtx;
    condition_variable  repo_not_full;
    condition_variable  repo_not_empty;
} *pRespository;


class FastqHandler {

    public:

        FastqHandler (const string& srcPath);

        ~FastqHandler ();

        void filter(unsigned int standard, double standard_ratio);// filter fastq data from given file path with the specified standard

    public:

        void gz_fetch_proc();

        void gz_write_proc();

    private:

        void buffAnalysis(string &buff);

        bool judgeTheSeq (dnaSeq& seq);// judge the dnaSeq'effectiveness

        static inline string getParrentPath(const string &path);

    private:
        string _srcPath;
        string _outPath;

        gzFile _fastqFile, _outFile;
        z_off_t _backOffset;

        unsigned int _filterStandard_score;
        double _filterStandard_ratios;

        std::function<void (void)> _fetchProc, _writeProc;
        //statistics data
        ulong _seqSum, _effectiveSum;

        // thread control
        atomic_bool _bFinishReading;

        // thread communicat
        buffRespository _repo;
};


#endif //FASTQ_GZ_HANDLER_FASTQREADER_H

/**
 *  data sample
 *  @CL100058413L1C017R096_574429
 *  GGTGAGTTGCTATTAACTATAATTTTGCTATCTTA
 *  +
 *  A>1D7?B<>FCFFEFFFBFEB6?F?CEE?BDCEAC       ---------> llumia(0, 41) pre+33
 **/
