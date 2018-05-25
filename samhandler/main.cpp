#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <algorithm>
#include "samtool_core.h"

void outputStaticstic (StasticsData& stat)
{
    std::cout << "sum : " << stat.sum << std::endl;
    std::cout << "effective Sum : " << stat.effectiveSum << std::endl;
    std::cout << "xt_U : " << stat.xtU << std::endl;
    std::cout << "xt_R : " << stat.xtR << std::endl;
    std::cout << "min MapQ : " << stat.minMapQ << std::endl;
    std::cout << "max MapQ : " << stat.maxMapQ << std::endl;
}

int main(int argc, char** argv)
{
    string srcPath;
    string outPath;
    FilterStandard cond;
    char *dataBuff;

    try {

        int ch;
        //opterr = 1;
        while ((ch = getopt(argc, argv, "f:x:n:m:o:")) != -1) {
            switch (ch) {
                case 'f':
                    srcPath = optarg;
                    break;
                case 'o':
                    outPath = optarg;
                    break;
                case 'x':
                    cond.xt = *optarg;
                    break;
                case 'n':
                    cond.nm = atoi(optarg);
                    break;
                case 'm':
                    cond.mapQ = atoi(optarg);
                default:    /* '?'*/
                    break;
            }
        }

        for (int index = optind; index < argc; ++index)
            printf("non-option argument %s\n", argv[index]);


        // allocate memory
        dataBuff = (char *) calloc(READ_BUFF_SIZE, sizeof(char));

//        // read data
        { SamReader(srcPath).readAll(dataBuff, READ_BUFF_SIZE); }

        // filte
        sortIndecVec indexVec;
        StasticsData stasticData;
        off64_t header_end_pos = 0;

        Filter(cond).goForAll(dataBuff, indexVec, stasticData, header_end_pos);

        // sort
        Sorter::quickSort(indexVec, indexVec.begin(), indexVec.end());
        /*std::sort(indexVec.begin(), indexVec.end(), [](const SamSortIndex& l, const SamSortIndex& r ){
          return  l.key < r.key;
        }); */

        // output
        Samwriter(outPath).writeAll(dataBuff, indexVec, header_end_pos);


        free(dataBuff);
        dataBuff = NULL;

        outputStaticstic(stasticData);
    }
    catch (...) {
        fprintf(stderr, "errors happened.\n");

        free(dataBuff);
        return 1;
    }

    return 0;
}

