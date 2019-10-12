//
// Created by chaos on 9/17/19.
//

#ifndef VANS_PMSEQ_OBEJECTWRAPPER_H
#define VANS_PMSEQ_OBEJECTWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct c_filter_fq c_filter_fq;
c_filter_fq* newFilter(const char* path);
int filter_seq(c_filter_fq* v, const char* seq, const char* qual);
int filter_seq_by_repeat(c_filter_fq* v, const char* seq, const char* qual);
int filter_seq_by_apear_times(c_filter_fq* v, const char* seq, const char* qual);


#ifdef __cplusplus
}
#endif


#endif //VANS_PMSEQ_OBEJECTWRAPPER_H
