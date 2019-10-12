//
// Created by chaos on 9/17/19.
//
#include "obejectWrapper.h"
#include "c_filter_fq.hpp"

extern "C" {
    c_filter_fq* newFilter(const char* path) {
        return new c_filter_fq(path);
    }

    int filter_seq(c_filter_fq* v, const char* seq, const char* qual) {
        return v->filter_out_e(seq, qual);
    }

    int filter_seq_by_repeat(c_filter_fq*v, const char* seq, const char* qual) {
        return v->filter_out_e_by_appear_times(seq, qual);
    }

    int filter_seq_by_apear_times(c_filter_fq* v, const char* seq, const char* qual) {
        return v->filter_out_e_by_appear_times(seq, qual);
    }
}
