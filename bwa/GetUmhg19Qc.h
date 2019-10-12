//
// Created by Pason on 2019-7-1.
//

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <map>
#include <fstream>



#ifndef BGI_GET_UMHG19QC_H
#define BGI_GET_UMHG19QC_H

using namespace std;



class GetUmhg19Qc{
	
public:
	GetUmhg19Qc(float Nnumper, float Q5per, int short_reads_all, int short_bases_all, int lowq_reads_all, int lowq_rds_bs_all
		, int N_reads_all, int N_rds_bs_all);

	
	void splitString(const string& s, vector<string>& v, const string& c);
	int creatFile(const char* filename, bool isclean);

	float getMaprate() const;
	float getNnumper() const;
	float getQ5per() const;
	
	int getUmRnum() const;
	int getUmBase() const;
	int getShortReadsNum() const;
	int getShortBasesNum() const;
	int getLowqReadsNum() const;
	int getLowqRdsBsNum() const;
	int getNReadsNum() const;
	int getNRdsBsNum() const;
	
	int getShortReadsAll() const;
	int getShortBasesAll() const;
	int getLowqReadsAll() const;
	int getLowqRdsBsAll() const;
	int getNReadsAll() const;
	int getNRdsBsAll() const;

	void setNnumper(const float Nnumper);
	void setQ5per(const float Q5per);
	void setMaprate(const float maprate);
		
	void setUmRnum(const int um_rnum);
	void setUmBase(const int um_base);
	void setShortReadsNum(const int short_reads_num);
	void setShortBasesNum(const int short_bases_num);
	void setLowqReadsNum(const int lowq_reads_num);
	void setLowqRdsBsNum(const int lowq_rds_bs_num);
	void setNReadsNum(const int N_reads_num);
	void setNRdsBsNum(const int N_rds_bs_num);
	
	void setShortReadsAll(const int short_reads_all);
	void setShortBasesAll(const int short_bases_all);
	void setLowqReadsAll(const int lowq_reads_all);
	void setLowqRdsBsAll(const int lowq_rds_bs_all);
	void setNReadsAll(const int N_reads_all);
	void setNRdsBsAll(const int N_rds_bs_all);

private:	
	float maprate;
	float Nnumper;
	float Q5per;

	int um_rnum;
	int um_base;
	int short_reads_num;
	int short_bases_num;
	int lowq_reads_num;
	int lowq_rds_bs_num;
	int N_reads_num;
	int N_rds_bs_num;
	
	int short_reads_all;
	int short_bases_all;
	int lowq_reads_all;
	int lowq_rds_bs_all;
	int N_reads_all;
	int N_rds_bs_all;
}; 


#endif
