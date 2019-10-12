
//
// Created by Pason on 2019-7-1.
//


#include "GetUmplasmid.h"


void GetUmplasmid::splitString(const string& s, vector<string>& v, const string& c)
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
int GetUmplasmid::creatFile(const char* filename, bool isclean){
	
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

void GetUmplasmid::tagOneTwo(const char * samfileName, string tag)
{
	char str[512];
    strncpy(str, samfileName, 512);
    int len=strlen(str);
	char fileName[128] = {0};
	memset(fileName, 0, 128);
	int i;
    for( i = len - 1; i > 0; i--)
    {
        if(str[i]=='/')
       	{ 
       		for( int j = i + 1, k = 0; j < len; j++, k++)
       		{
       			fileName[k] = str[j];
       		}
			break;
        }
    }
	
	if(strstr(fileName, "fq1") != NULL){
		tag = "_1";
	}
	else if(strstr(fileName, "fq2") != NULL){
		//strcpy(tag, "_2");
		tag = "_2";
	}	

}

int GetUmplasmid::sumDigitBeforeS(string &numS)
{
	
	//perl -e '@t=(); @t="71S81S"=~/(\d+)[S]/g; print @t;'

	vector<string> snum;
	string::size_type pos1, pos2;
	pos2 = 0;
	pos1 = 0;
	for(int i = 0 ; i < numS.size(); i++){
		if(isdigit(numS[i])){
			pos2++;
			continue;
		}
		if(numS[i] == 'S')
		{
			snum.push_back(numS.substr(pos1,pos2-pos1));
		}
		pos2++;
		pos1 = pos2;
	}
	int ssum = 0;
	for(int i = 0; i < snum.size(); i++){
		 ssum += atoi(snum[i].c_str());
	}
	return ssum;
}


float GetUmplasmid::getMaprate() const{
	return maprate;
}

float GetUmplasmid::getMisrate() const{
	return misrate;
}

int GetUmplasmid::getShortReadsNum() const{
	return short_reads_num;
}

int GetUmplasmid::getShortBasesNum() const{
	return short_bases_num;
}


void GetUmplasmid::setMaprate(const float maprate){
	GetUmplasmid::maprate = maprate;
}

void GetUmplasmid::setMisrate(const float misrate){
	GetUmplasmid::misrate = misrate;
}


void GetUmplasmid::setShortReadsNum(const int short_reads_num){
 	GetUmplasmid::short_reads_num = short_reads_num;
}

void GetUmplasmid::setShortBasesNum(const int short_bases_num){
	 GetUmplasmid::short_bases_num = short_bases_num;
}


