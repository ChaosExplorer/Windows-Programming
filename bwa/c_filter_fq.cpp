/*
 * jd create at 2019-07-08
 * to build & run on Linux, just run :
 * make run
 */
//#pragma once
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

#include "c_filter_fq.hpp"


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



#if 1 

// begin static initialize area
uint64_t c_filter_fq::table_crc[sz_crc];

const unordered_map<char, char> c_filter_fq::dic_ATGC { { 'G', 'C' },{ 'A','T' },{ 'T','A' },{ 'C','G' },{'N','N'} };

int c_filter_fq::derepmin = 100;
int c_filter_fq::derep = 14; 
int c_filter_fq::lc_threshold = 7; 
string c_filter_fq::lc_method = string("dust"); 
string c_filter_fq::out_bad = string("null"); 
// end static initialize area


// cpp_ start 
c_filter_fq::c_filter_fq(const string &fn_config)
{
    read_config(fn_config);
    generate_crc_table();
}

void c_filter_fq::read_config(const string & fn_config)
{
/*  
perl $t/prinseq-lite.pl -derep 14 -derep_min 100 -lc_method dust -lc_threshold 7 -out_bad null -fastq $t/18B0000399-1-11.umhg19.fq -log $t/18B0000399-1-11.umhg19.fq.clean.log -out_good $t/18B0000399-1-11.umhg19.fq.clean 
*/

    ifstream if_config(fn_config.c_str()); assert(if_config.is_open()); 
    string e_str; 

    auto trim_string = [](std::string const &s)
    {
        if (s.empty()) 
        {
            return s;
        }

        auto loc_s = s.find_first_not_of(" ");
        auto loc_e = s.find_last_not_of(" ") + 1;
        return s.substr(loc_s, loc_e); 
    };


    unordered_map<string, string> map_param_k_v; 


    while(std::getline(if_config, e_str))
    {
        int cnt_eq_symbol = 0; 
        int loc_eq_symbol = 0; 
        int loc_p = 0; 
        e_str = trim_string(e_str);
        if (e_str[0] == '#' || e_str.size() <= 1) 
        { continue; }

        for(auto &e_ : e_str)
        {
            if(e_ == '=')
            {
                cnt_eq_symbol++; 
                loc_eq_symbol = loc_p; 
            }
            loc_p++; 
        }

        if (1 == cnt_eq_symbol)
        {
            //cout << e_str << " : "<<  loc_eq_symbol << endl; 
            string left_exp = e_str.substr(0, loc_eq_symbol); 
            string right_exp = e_str.substr(loc_eq_symbol+1); 

            auto left_exp_ = trim_string(left_exp); 
            auto right_exp_ = trim_string(right_exp); 

            assert(left_exp_.size() > 0); 
            assert(right_exp_.size() > 0); 

            map_param_k_v[left_exp_] = right_exp_; 
        }
        else
        {
            std::cerr << "- please make sure your filter.config has format [ key = value ] " << endl; 
            assert(1 == cnt_eq_symbol); 
        }
    }

    if_config.close(); 

#if 0 
    for(auto e: map_param_k_v)
    {
        cout << e.first << ":" << e.second << endl; 
    }
#endif


    string lc_method = map_param_k_v["lc_method"]; 
    auto len_lc_method = lc_method.size(); 
    assert(lc_method[0] = '"'); 
    assert(lc_method[len_lc_method-1] = '"'); 

    string out_bad = map_param_k_v["out_bad"]; 
    auto len_out_bad = out_bad.size(); 
    assert(out_bad[0] = '"'); 
    assert(out_bad[len_out_bad-1] = '"'); 

    // truely set the static var
    c_filter_fq::derepmin = atoi(map_param_k_v["derepmin"].c_str());
    c_filter_fq::derep = atoi(map_param_k_v["derep"].c_str()); 
    c_filter_fq::lc_threshold = atoi(map_param_k_v["lc_threshold"].c_str()); 

    c_filter_fq::lc_method = lc_method.substr(1, len_lc_method-2); 
    c_filter_fq::out_bad = out_bad.substr(1, len_out_bad-2); 
}



int c_filter_fq::map_md5all_exist_k0_k1(uint64_t k0, int k1)
{
    if (-1 == k1)
    {
        if (md5all.find(k0) != md5all.end())
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (md5all.find(k0) != md5all.end())
    {
        if (md5all[k0].find(k1) != md5all[k0].end())
        {
            return 1;
        }
    }
    return 0;
};

void c_filter_fq::generate_crc_table()
{
    const uint64_t poly = 0xC96C5795D7870F42;
    for (int i = 0; i<c_filter_fq::sz_crc; ++i)
    {
        uint64_t crc = i;

        for (int j = 0; j < 8; ++j)
        {
            // is current coefficient set?
            if (crc & 1)
            {
                // yes, then assume it gets zero'd (by implied x^64 coefficient of dividend)
                crc >>= 1;

                // and add rest of the divisor
                crc ^= poly;
            }
            else
            {
                // no? then move to next coefficient
                crc >>= 1;
            }
        }

        c_filter_fq::table_crc[i] = crc;
    }
}

uint64_t c_filter_fq::calculate_crc(const string& stream)
{

    uint64_t crc = 0;
    auto n = stream.size();
    for (int i = 0; i<n; ++i)
    {
        unsigned char index = (unsigned char)(stream[i] ^ crc);
        uint64_t lookup = c_filter_fq::table_crc[index];

        crc >>= 8;
        crc ^= lookup;
    }

    return crc;
}

int c_filter_fq::filter_out_e_by_repeat(const string& v_s1, const string& v_s2)
{
    // 0, not filter out; 1, filter out
    int ret_filter_out_code = 0;
    const int WINDOWSIZE = 64;
    const int WINDOWSTEP = 32;
    const float POINTFIVE = 0.5f;
    const float COMPLVAL = 7.0f;

    int good = 1;


    auto seq = string(v_s1);
    auto &qual = v_s2;

    std::transform(seq.begin(), seq.end(), seq.begin(), ::toupper);
    auto &seqn = seq;

#if 0
    if (seqn == "ATGATAACCATCAAACATCCACAAAATTATATAAAACAAAAATACAGGAC")
    {
        int tt = 99;
    }
#endif

    auto length = seqn.size();
    int rest = 0;
    int steps = 0;

    if(length <= WINDOWSIZE) {
        rest = length;
        steps = 0;
    }
    else
    {
        steps = (int)((length - WINDOWSIZE) * 1.0f / WINDOWSTEP) + 1;
        rest = length - steps * WINDOWSTEP;
        if( rest > WINDOWSTEP )
        {}
        else
        {
            rest +=  WINDOWSTEP;
            steps--;
        }
    }

    auto num = WINDOWSIZE - 2;
    auto bynum = 1.0f / num;
    num--;
    auto mean = 0.0f;

    // cal "dust"
    float dustscore = 0.0f;
    vector<float> v_vals{};

    // 1
    for (int i = 0; i < steps; i++)
    {
        auto str = seqn.substr((i * WINDOWSTEP), WINDOWSIZE);
        //%counts = ();
        unordered_map<string, int> counts;
        for (int i = 0; i < 62; i++) {
            counts[str.substr(i, 3)]++;
        }

        dustscore = 0;
        for (auto &e : counts)
        {
            auto &s_ = e.second;
            dustscore += (s_ * (s_ - 1) * POINTFIVE);
        }

        v_vals.push_back(dustscore * bynum);
    }

    // 2
    if (rest > 5)
    {
        auto str = seqn.substr((steps * WINDOWSTEP), rest);
        unordered_map<string, int> counts;
        num = rest - 2;
        for (int i = 0; i < num; i++) {
            counts[str.substr(i, 3)]++;
        }

        dustscore = 0;
        for (auto &e : counts)
        {
            auto &s_ = e.second;
            dustscore += (s_ * (s_ - 1) * POINTFIVE);
        }

        auto last_val = ((dustscore / (num - 1)) * ((WINDOWSIZE - 2) / (float)num));
        v_vals.push_back(last_val);
    }

    auto mean_ = accumulate(v_vals.begin(), v_vals.end(), 0.0) / v_vals.size();

    if ((int)(mean_ * 100 / 31) > COMPLVAL)
    {
        ret_filter_out_code = 1;
    }

    return ret_filter_out_code;
}

int c_filter_fq::filter_out_e_by_appear_times(const string& v_s1, const string& v_s2)
{
    int ret_filter_out_code = 0;
    auto seq = string(v_s1);
    auto &qual = v_s2;

    std::transform(seq.begin(), seq.end(), seq.begin(), ::toupper);
    auto &seqn = seq;

    if (0 == ret_filter_out_code)
    {
        auto crc_val = calculate_crc(seqn);
        auto &md5 = crc_val;
        auto flag_md_exist = map_md5all_exist_k0_k1(md5, -1);

        if (!flag_md_exist)
        {
            md5all[md5] = { {0 , 0}, {3, 0} };
        }

        if (flag_md_exist)
        {
            auto& cnt_md5_k_0 = md5all[md5][0];
            cnt_md5_k_0++;
            if (c_filter_fq::derepmin <= cnt_md5_k_0 + 1)
            {
                ret_filter_out_code = 2;
            }
        }

        if (0 == ret_filter_out_code)
        {
            string seqn_r = string(seqn);
            std::reverse(seqn_r.begin(), seqn_r.end());

            for (auto& e : seqn_r)
            {
                e = c_filter_fq::dic_ATGC.at(e);
            }

            auto md5r = calculate_crc(seqn_r);
            if (map_md5all_exist_k0_k1(md5r, -1))
            {
                auto& cnt_md5r_k_3 = md5all[md5r][3];
                cnt_md5r_k_3++;
                if (c_filter_fq::derepmin <= cnt_md5r_k_3 + 1)
                {
                    ret_filter_out_code = 3;
                }
            }
        }
    }

    return ret_filter_out_code;
}

int c_filter_fq::filter_out_e(const string& v_s1, const string& v_s2)
{
    // 0, not filter out; 1, filter out
    int ret_filter_out_code = 0;
    const int WINDOWSIZE = 64;
    const int WINDOWSTEP = 32;
    const float POINTFIVE = 0.5f;
    const float COMPLVAL = 7.0f;

    int good = 1;


    auto seq = string(v_s1);
    auto &qual = v_s2;

    std::transform(seq.begin(), seq.end(), seq.begin(), ::toupper);
    auto &seqn = seq;

#if 0
    if (seqn == "ATGATAACCATCAAACATCCACAAAATTATATAAAACAAAAATACAGGAC")
    {
        int tt = 99; 
    }
#endif 

    auto length = seqn.size();
    int rest = 0; 
    int steps = 0; 

    if(length <= WINDOWSIZE) {
        rest = length;
        steps = 0;
    } 
    else
    {
        steps = (int)((length - WINDOWSIZE) * 1.0f / WINDOWSTEP) + 1;
        rest = length - steps * WINDOWSTEP;
		if( rest > WINDOWSTEP )
		{}
		else 
        {
            rest +=  WINDOWSTEP;  
            steps--; 
        }
    }

    auto num = WINDOWSIZE - 2;
    auto bynum = 1.0f / num;
    num--;
    auto mean = 0.0f;

    // cal "dust"
    float dustscore = 0.0f;
    vector<float> v_vals{};

    // 1
    for (int i = 0; i < steps; i++) 
    {
        auto str = seqn.substr((i * WINDOWSTEP), WINDOWSIZE);
        //%counts = ();
        unordered_map<string, int> counts;
        for (int i = 0; i < 62; i++) {
            counts[str.substr(i, 3)]++;
        }

        dustscore = 0;
        for (auto &e : counts)
        {
            auto &s_ = e.second;
            dustscore += (s_ * (s_ - 1) * POINTFIVE);
        }

        v_vals.push_back(dustscore * bynum);
    }

    // 2
    if (rest > 5)
    {
        auto str = seqn.substr((steps * WINDOWSTEP), rest);
        unordered_map<string, int> counts;
        num = rest - 2;
        for (int i = 0; i < num; i++) {
            counts[str.substr(i, 3)]++;
        }

        dustscore = 0;
        for (auto &e : counts)
        {
            auto &s_ = e.second;
            dustscore += (s_ * (s_ - 1) * POINTFIVE);
        }

        auto last_val = ((dustscore / (num - 1)) * ((WINDOWSIZE - 2) / (float)num));
        v_vals.push_back(last_val);
    }

    auto mean_ = accumulate(v_vals.begin(), v_vals.end(), 0.0) / v_vals.size();

    if ((int)(mean_ * 100 / 31) > COMPLVAL) 
    {
        ret_filter_out_code = 1;
    }

    if (0 == ret_filter_out_code)
    {
        auto crc_val = calculate_crc(seqn); 
        auto &md5 = crc_val;
        auto flag_md_exist = map_md5all_exist_k0_k1(md5, -1);

        if (!flag_md_exist)
        {
            md5all[md5] = { {0 , 0}, {3, 0} };
        }

        if (flag_md_exist)
        {
            auto& cnt_md5_k_0 = md5all[md5][0];
            cnt_md5_k_0++;
            if (c_filter_fq::derepmin <= cnt_md5_k_0 + 1)
            {
                ret_filter_out_code = 2;
            }
        }

        if (0 == ret_filter_out_code)
        {
            string seqn_r = string(seqn);
            std::reverse(seqn_r.begin(), seqn_r.end());

            for (auto& e : seqn_r)
            {
                e = c_filter_fq::dic_ATGC.at(e);               
            }

            auto md5r = calculate_crc(seqn_r);
            if (map_md5all_exist_k0_k1(md5r, -1))
            {
                auto& cnt_md5r_k_3 = md5all[md5r][3];
                cnt_md5r_k_3++;
                if (c_filter_fq::derepmin <= cnt_md5r_k_3 + 1)
                {
                    ret_filter_out_code = 3;
                }
            }
        }
    }

    return ret_filter_out_code;
}

#endif 
