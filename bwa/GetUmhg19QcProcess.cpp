//
// Created by Pason on 2019-7-1.
//

#include "GetUmhg19QcProcess.h"

#define BUF_SIZE 512

void GetUmhg19QcProcess::filterStat(const char * samfileName, const int fileFd, GetUmhg19Qc & getUmhg19Qc)
{
	char str[BUF_SIZE];
    strncpy(str, samfileName, BUF_SIZE);
	//cout << "fileFd: " << fileFd << endl;
    int len=strlen(str);
	char fileName[BUF_SIZE] = {0};
	memset(fileName, 0, BUF_SIZE);
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
	
	char tag [3] = {0};
	if(strstr(fileName, "fq1") != NULL){
		strcpy(tag, "_1");
	}
	else if(strstr(fileName, "fq2") != NULL){
		strcpy(tag, "_2");
	}	

	getUmhg19Qc.setUmRnum(0);
	getUmhg19Qc.setUmBase(0);
	getUmhg19Qc.setShortReadsNum(0);
	getUmhg19Qc.setShortBasesNum(0);
	getUmhg19Qc.setLowqReadsNum(0);
	getUmhg19Qc.setLowqRdsBsNum(0);
	getUmhg19Qc.setNReadsNum(0);
	getUmhg19Qc.setNRdsBsNum(0);

	ifstream samFileRead;
	samFileRead.open(samfileName);
	string samFileLine;
	map<string,int> bgiMap;


	while(getline(samFileRead, samFileLine)){
		if(samFileLine.find("@", 0) == 0){
			continue;
		}
		vector<string> bgiv;
		getUmhg19Qc.splitString(samFileLine, bgiv, "\t");
		if(bgiv.size() < 2 || atoi(bgiv[1].c_str()) >= 2048){continue;}

		int bgiv9Len = bgiv[9].length();
		if(35 > bgiv9Len){
			getUmhg19Qc.setShortReadsNum(getUmhg19Qc.getShortReadsNum() + 1);
			getUmhg19Qc.setShortBasesNum(getUmhg19Qc.getShortBasesNum() + bgiv9Len);
			continue;
		}

		//next if(exists $count{$a[0]}); 
		if (bgiMap.count(bgiv[0]) == 1){continue;} //1 exists ，0 not exists
			
		//perl -e '@t=(); @t="71S81S"=~/(\d+)[S]/g; print @t;'
		vector<string> snum;
		string::size_type pos1, pos2;
    	pos2 = 0;
   	 	pos1 = 0;
		for(int i = 0 ; i < bgiv[5].size(); i++){
			if(isdigit(bgiv[5][i])){
				pos2++;
				continue;
			}
			if(bgiv[5][i] == 'S')
			{
				snum.push_back(bgiv[5].substr(pos1,pos2-pos1));
			}
			pos2++;
			pos1 = pos2;
		}

		int ssum = 0;
		for(int i = 0; i < snum.size(); i++){
			 ssum += atoi(snum[i].c_str());
		}
		
		string quality = bgiv[10];
		string seq = bgiv[9];

		if(string::npos != bgiv[2].find('*')  || atoi(bgiv[1].c_str()) == 4 
			|| ((ssum != 0 && ((float)ssum / (float)bgiv9Len ) > (float)1 - getUmhg19Qc.getMaprate()))){
		//1.Quality rate(<Q5) >= 0.5
		
			int lowqb_num = 0;
			for(int i = 0; i < quality.size(); i++){
				if( 5 >= quality[i]  - 33 ){lowqb_num++;} //stupid who write the perl
			}
			if((float)lowqb_num / (float)bgiv9Len >= getUmhg19Qc.getQ5per()){
				getUmhg19Qc.setLowqReadsNum(getUmhg19Qc.getLowqReadsNum() + 1);
				getUmhg19Qc.setLowqRdsBsNum(getUmhg19Qc.getLowqRdsBsNum() + bgiv9Len);
				continue;
			}

		//2.N.rate > 0.1

			int N_num = 0;
			for(int i = 0; i < seq.size(); i++){
				if('N' == seq[i] || seq[i] == 'n'){
					N_num++;
				}
			}

			if(10 <= N_num || (float)N_num / (float)bgiv9Len >= getUmhg19Qc.getNnumper()){
				getUmhg19Qc.setNReadsNum(getUmhg19Qc.getNReadsNum() + 1 );
				getUmhg19Qc.setNRdsBsNum(getUmhg19Qc.getNRdsBsNum() + bgiv9Len);
				continue;
			}
			
			bgiMap.insert(make_pair(bgiv[0],1));
			getUmhg19Qc.setUmRnum(getUmhg19Qc.getUmRnum() + 1);
			getUmhg19Qc.setUmBase(getUmhg19Qc.getUmBase() + bgiv9Len);

			//print $ot "\@$a[0]$tag\n$a[9]\n+\n$a[10]\n";
			char  bufFileFd[2048] = {0};
			sprintf(bufFileFd, "@%s%s\n%s\n+\n%s\n", bgiv[0].c_str(), tag, bgiv[9].c_str(), bgiv[10].c_str());
			//cout << "bufFileFd: " << bufFileFd << endl;
			write(fileFd, bufFileFd, strlen(bufFileFd));
		}
		
	}
	samFileRead.close();
}

GetUmhg19QcProcess::GetUmhg19QcProcess(float mapRate, int thdNum): thread_num(thdNum), mapRate(mapRate) {
    initial();
}

void GetUmhg19QcProcess::initial()
{
    getUmhg19Qc = new GetUmhg19Qc(NNUMPER, Q5PER, 0, 0, 0, 0, 0, 0);

    getUmhg19Qc->setMaprate(mapRate);
}

int GetUmhg19QcProcess::outputStat(char* statFilePath)
{

	GetUmhg19Qc getUmhg19Qc(NNUMPER, Q5PER, 0, 0, 0, 0, 0, 0);
	//
	int total_um_rnum = 0;
	int total_um_base = 0;
	
#if 1

	total_um_rnum += getUmhg19Qc.getUmRnum();
	total_um_base += getUmhg19Qc.getUmBase();

	getUmhg19Qc.setShortReadsAll(getUmhg19Qc.getShortReadsAll() + getUmhg19Qc.getShortReadsNum());
	getUmhg19Qc.setShortBasesAll(getUmhg19Qc.getShortBasesAll() + getUmhg19Qc.getShortBasesNum());
	getUmhg19Qc.setLowqReadsAll(getUmhg19Qc.getLowqReadsAll() + getUmhg19Qc.getLowqReadsNum());
	getUmhg19Qc.setLowqRdsBsAll(getUmhg19Qc.getLowqRdsBsAll() + getUmhg19Qc.getLowqRdsBsNum());
	getUmhg19Qc.setNReadsAll(getUmhg19Qc.getNReadsAll() + getUmhg19Qc.getNReadsNum());
	getUmhg19Qc.setNRdsBsAll(getUmhg19Qc.getNRdsBsAll() + getUmhg19Qc.getNRdsBsNum());


	char statFileBuf[STATFILEBUFSIZE];
	memset(statFileBuf, 0, STATFILEBUFSIZE);

	int statFileFd = getUmhg19Qc.creatFile(statFilePath, false);
	if(0 > statFileFd) { perror(statFilePath);}

	lseek(statFileFd, 0, SEEK_END);
	sprintf(statFileBuf, "%s %d\n%s %d\n", "unmaphg19 reads number:", total_um_rnum, "nunmaphg19 reads base:", total_um_base);
	cout << "statFileBuf:" << statFileBuf; 
	write(statFileFd, statFileBuf, strlen(statFileBuf));
	close(statFileFd);
	
	int filter_total = getUmhg19Qc.getShortReadsAll() + getUmhg19Qc.getLowqReadsAll() + getUmhg19Qc.getNReadsAll();
	printf("low_quality_reads: %d \nlow_quality_base: %d \nN_reads: %d \nN_reads_bases: %d \nfiltered_reads: %d \n", getUmhg19Qc.getLowqReadsAll(), getUmhg19Qc.getLowqRdsBsAll(), 
		getUmhg19Qc.getNReadsAll(), getUmhg19Qc.getLowqRdsBsAll(), filter_total);
#endif
	
    return 0;
}


