
//
// Created by Pason on 2019-7-1.
//


#include "GetUmhg19Qc.h"

GetUmhg19Qc::GetUmhg19Qc(float Nnumper, float Q5per, int short_reads_all, int short_bases_all, int lowq_reads_all, int lowq_rds_bs_all
		, int N_reads_all, int N_rds_bs_all){
	
	this->Nnumper = Nnumper;
	this->Q5per = Q5per;
	this->short_reads_all = short_reads_all;
	this->short_bases_all = short_bases_all;
	this->lowq_reads_all = lowq_reads_all;
	this->lowq_rds_bs_all = lowq_rds_bs_all;
	this->N_reads_all = N_reads_all;
	this->N_rds_bs_all = N_rds_bs_all;
}

void GetUmhg19Qc::splitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}


//creat file return fd,you need close it on yourself
int GetUmhg19Qc::creatFile(const char* filename, bool isclean){
	
	if( 0 == access(filename, F_OK) && isclean == true){//file exist, clean it
		int fd = open(filename, O_RDWR, 0666);
		ftruncate(fd, 0); 
		lseek(fd, 0, SEEK_SET);
		return fd;
	}

	if( 0 == access(filename, F_OK) && isclean == false){//file exist, lseek tail
		int fd = open(filename, O_RDWR, 0666);
		//lseek(fd, 0, SEEK_END);
		return fd;
	}
	
	//file not exist, creat it
	int i, len;
    char str[512];
    strncpy(str, filename, 512);
    len=strlen(str);
    for(i=0; i<len; i++)
    {
        if(str[i]=='/')
        {
            str[i] = '\0';
            if(access(str, 0) !=0)  // Folder not exist 
                mkdir(str, 0777);
            str[i] = '/';
        }
    }
    if( len > 0 && access(str, 0) != 0){ //creat file
        return (open(str, O_CREAT|O_RDWR, 0666));
	}
}

float GetUmhg19Qc::getMaprate() const{
	return maprate;
}


float GetUmhg19Qc::getNnumper() const{
	return Nnumper;
}
	
float GetUmhg19Qc::getQ5per() const{
	return Q5per;
}

int GetUmhg19Qc::getUmRnum() const {
    return um_rnum;
}

int GetUmhg19Qc::getUmBase() const{
	return um_base;
}

int GetUmhg19Qc::getShortReadsNum() const{
	return short_reads_num;
}

int GetUmhg19Qc::getShortBasesNum() const{
	return short_bases_num;
}

int GetUmhg19Qc::getLowqReadsNum() const{
	return lowq_reads_num;
}

int GetUmhg19Qc::getLowqRdsBsNum() const{
	return lowq_rds_bs_num;
}

int GetUmhg19Qc::getNReadsNum() const{
	return N_reads_num;
}

int GetUmhg19Qc::getNRdsBsNum() const{
	return N_rds_bs_num;
}


int GetUmhg19Qc::getShortReadsAll() const{
	return short_reads_all;
}

int GetUmhg19Qc::getShortBasesAll() const{
	return short_bases_all;
}

int GetUmhg19Qc::getLowqReadsAll() const{
	return lowq_reads_all;
}

int GetUmhg19Qc::getLowqRdsBsAll() const{
	return lowq_rds_bs_all;
}

int GetUmhg19Qc::getNReadsAll() const{
	return N_reads_all;
}

int GetUmhg19Qc::getNRdsBsAll() const{
	return N_rds_bs_all;
}


void GetUmhg19Qc::setMaprate(const float maprate){
	GetUmhg19Qc::maprate = maprate;
}

void GetUmhg19Qc::setNnumper(const float Nnumper){
	GetUmhg19Qc::Nnumper = Nnumper;
}

void GetUmhg19Qc::setQ5per(const float Q5per){
	GetUmhg19Qc::Q5per = Q5per;
}


void GetUmhg19Qc::setUmRnum(const int um_rnum) {
    GetUmhg19Qc::um_rnum = um_rnum;
}

void GetUmhg19Qc::setUmBase(const int um_base){
	 GetUmhg19Qc::um_base = um_base;
}

void GetUmhg19Qc::setShortReadsNum(const int short_reads_num){
 	GetUmhg19Qc::short_reads_num = short_reads_num;
}

void GetUmhg19Qc::setShortBasesNum(const int short_bases_num){
	 GetUmhg19Qc::short_bases_num = short_bases_num;
}

void GetUmhg19Qc::setLowqReadsNum(const int lowq_reads_num){
	 GetUmhg19Qc::lowq_reads_num = lowq_reads_num;
}

void GetUmhg19Qc::setLowqRdsBsNum(const int lowq_rds_bs_num){
	 GetUmhg19Qc::lowq_rds_bs_num = lowq_rds_bs_num;
}

void GetUmhg19Qc::setNReadsNum(const int N_reads_num){
	 GetUmhg19Qc::N_reads_num = N_reads_num;
}

void GetUmhg19Qc::setNRdsBsNum(const int N_rds_bs_num){
	 GetUmhg19Qc::N_rds_bs_num = N_rds_bs_num;
}


void GetUmhg19Qc::setShortReadsAll(const int short_reads_all){
	GetUmhg19Qc::short_reads_all = short_reads_all;
}

void GetUmhg19Qc::setShortBasesAll(const int short_bases_all){
	GetUmhg19Qc::short_bases_all = short_bases_all;
}

void GetUmhg19Qc::setLowqReadsAll(const int lowq_reads_all){
	GetUmhg19Qc::lowq_reads_all = lowq_reads_all;	
}

void GetUmhg19Qc::setLowqRdsBsAll(const int lowq_rds_bs_all){
	GetUmhg19Qc::lowq_rds_bs_all = lowq_rds_bs_all;	
}

void GetUmhg19Qc::setNReadsAll(const int N_reads_all){
	GetUmhg19Qc::N_reads_all = N_reads_all;
}

void GetUmhg19Qc::setNRdsBsAll(const int N_rds_bs_all){
	GetUmhg19Qc::N_rds_bs_all = N_rds_bs_all;	
}

