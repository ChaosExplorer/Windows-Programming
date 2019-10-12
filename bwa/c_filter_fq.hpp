/*
 * jd create at 2019-07-08
 * to build & run on Linux, just run :
 * make run
 */
#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4018)

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>  // uint64_t
#include <assert.h>

#include <vector>
#include <sstream>
#include <unordered_map>
#include <map>
#include <algorithm>    // std::sort
#include <numeric>
#include <utility>

#include <stack>
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <thread>
#include <mutex>



#ifdef __linux__
#define __P__  return 0;   //__
#else
#define __P__  system("pause");return 0;   //__
#define popen(fp, fr) _popen((fp),(fr))
#define pclose(fp) _pclose(fp)
#define sleep(i) Sleep(1000*(i))
#endif
using namespace std;


#define DEL_ARR_MEM(P) if(NULL != (P)){delete [] (P); (P) = NULL;}



class c_filter_fq {
    public:
        c_filter_fq(const string &fn_config);  

    public:
        static const int sz_crc = 256;
        static uint64_t table_crc[sz_crc];
        unordered_map< uint64_t, unordered_map<int, int> > md5all; 
        static const unordered_map<char, char> dic_ATGC;

        static int derepmin;
        static int derep; 
        static string lc_method; 
        static int lc_threshold; 
        static string out_bad; 

        int filter_out_e(const string& v_s1, const string& v_s2);
        int filter_out_e_by_repeat(const string& v_s1, const string& v_s2);
        int filter_out_e_by_appear_times(const string& v_s1, const string& v_s2);

    private:
        c_filter_fq(){} 
        void read_config(const string & fn_config);
        void generate_crc_table();
        int map_md5all_exist_k0_k1(uint64_t k0, int k1=-1);
        uint64_t calculate_crc(const string& stream);
        
        
};
