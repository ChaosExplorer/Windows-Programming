#include <iostream>
#include <unistd.h>
#include <cstring>
#include "samtool_core.h"

#define READ_BUFF_SIZE 2500000000

int main(int argc, char** argv) {

    string srcPath;
    string outPath;
    FilterStandard cond;
    char *dataBuff;

    try {

        int ch;
        //opterr = 1;
        while ((ch = getopt(argc, argv, "f:x:n:m:")) != -1) {
            switch (ch) {
                case 'f':
                    srcPath = optarg;
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

        // read data
        SamReader(srcPath).readAll(dataBuff, READ_BUFF_SIZE);

        // filte
        sortIndecVec indexVec;
        StasticsData stasticData;
        Filter(cond).goForAll(dataBuff, indexVec, stasticData);

        // sort
        Sorter::quickSort(indexVec, indexVec.begin(), indexVec.end());

        // output
        Samwriter(outPath).writeAll(dataBuff, indexVec);

        free(dataBuff);
        dataBuff = NULL;
    }
    catch (...) {
        fprintf(stderr, "errors happened.\n");

        free(dataBuff);
        return 1;
    }

    return 0;
}