// FastqReader.h
// Author : chaos
// Date :   5/4/18. //

#ifndef FASTQ_GZ_HANDLER_FASTQREADER_H
#define FASTQ_GZ_HANDLER_FASTQREADER_H

#include <string>
#include "zconf.h"
#include "zlib.h"
using std::string;

#define SEQLENGTH 37
#define IDLENGTH 30
#define MAX_LENGTH 128

// object struct to save the raw fastq data
typedef struct dnaSeq {
    char id[IDLENGTH];
    char baseSeq[SEQLENGTH];
    char quality[SEQLENGTH];
} *pDnaSeq;


class FastqHandler {
    public:
        FastqHandler (const string& srcPath);

        ~FastqHandler ();

        void filter(unsigned int standard, double standard_ratio);// filter fastq data from given file path with the specified standard

        bool gz_getline (string &str);// read a line from gz file.

        bool gz_readSeq (dnaSeq& seq);// read a dnaSeq from the source gzfile

        bool gz_wrieSeq (dnaSeq& seq);// write a dnaSeq into the output gzfile

        bool judgeTheSeq (dnaSeq& seq);// judge the dnaSeq'effectiveness

        static string getParrentPath(const string &path);

    private:
        string _srcPath;
        string _outPath;
        gzFile _fastqFile, _outFile;
        unsigned int _filterStandard_score;
        double _filterStandard_ratios;
};


#endif //FASTQ_GZ_HANDLER_FASTQREADER_H

/**
 *  data sample
 *  @CL100058413L1C001R001_100
 *  GGTGAGTTGCTATTAACTATAATTTTGCTATCTTA
 *  +
 *  A>1D7?B<>FCFFEFFFBFEB6?F?CEE?BDCEAC       ---------> llumia(0, 41) pre+33
 **/
