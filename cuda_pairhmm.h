//
// Created by nagato on 4/4/19.
//

#ifndef CUDACC_CUDA_PAIRHMM_H
#define CUDACC_CUDA_PAIRHMM_H


namespace cuda_pairhmm {

    struct Read {
        int length;
        const char* bases;
        const char* q;
        const char* i;
        const char* d;
        const char* c;
    };

    struct Haplotype {
        int length;
        const char* bases;
    };

    struct Batch {
        int num_reads;
        int num_haps;
        long num_cells;
        Read* reads;
        Haplotype* haps;
        float* results;
    };

    extern bool calculate(Batch& batch);

};


#endif //CUDACC_CUDA_PAIRHMM_H
