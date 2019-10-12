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

class GetUmplasmid{
	
public:
	//GetUmplasmid();
	
	void splitString(const string& s, vector<string>& v, const string& c);
	int creatFile(const char* filename, bool isclean);

	float getMaprate() const;
	float getMisrate() const;

	int getShortReadsNum() const;
	int getShortBasesNum() const;

	void setMaprate(const float maprate);
	void setMisrate(const float misrate);

	void setShortReadsNum(const int short_reads_num);
	void setShortBasesNum(const int short_bases_num);

	void tagOneTwo(const char * samfileName, string tag);
	int sumDigitBeforeS(string &numS);
	

private:	
	float maprate;
	float misrate;

	int short_reads_num;
	int short_bases_num;

}; 


#endif
