//
// Created by Pason on 2019-7-1.
//

#include "GetUmplasmidprocess.h"
#include<cstring>
using  namespace std;

void GetUmplasmidprocess::filterStat(const char * samfileName, const int fileFd, GetUmplasmid & GetUmplasmid)
{
	GetUmplasmid.setShortReadsNum(0);
	GetUmplasmid.setShortBasesNum(0);
	string tag;
	GetUmplasmid.tagOneTwo(samfileName, tag);
	ifstream samFileRead;
	samFileRead.open(samfileName);
	string samFileLine;
	map<string,int> bgiMap;
	
	while(getline(samFileRead, samFileLine)){
	
		if(samFileLine.find("@", 0) == 0){
			continue;
		}
		vector<string> bgiv;
		GetUmplasmid.splitString(samFileLine, bgiv, "\t");
		if(bgiv.size() < 2 || atoi(bgiv[1].c_str()) >= 2048){continue;}

		int bgiv9Len = bgiv[9].length();

		//next if(exists $count{$a[0]}); 
		if (bgiMap.count(bgiv[0]) == 1){continue;} //1 exists ，0 not exists

		char  bufFileFd[FILEBUFSIZE] = {0};
		if(string::npos != bgiv[2].find('*')  || atoi(bgiv[1].c_str()) == 4 ){
			bgiMap.insert(make_pair(bgiv[0],1));
			GetUmplasmid.setShortReadsNum(GetUmplasmid.getShortReadsNum() + 1);
			GetUmplasmid.setShortBasesNum(GetUmplasmid.getShortBasesNum() + bgiv9Len);	
			memset(bufFileFd, 0, FILEBUFSIZE);
			sprintf(bufFileFd, "@%s%s\n%s\n+\n%s\n", bgiv[0].c_str(), tag.c_str(), bgiv[9].c_str(), bgiv[10].c_str());
			write(fileFd, bufFileFd, strlen(bufFileFd));
		}
		else{
			
			//perl -e '@t=(); @t="71S81S"=~/(\d+)[S]/g; print @t;'
			int ssum = GetUmplasmid.sumDigitBeforeS(bgiv[5]);
		
			//my ($errnum)=$_=~/NM:i:(\d+)/;
			int errnum;
				
			for(int i = 0; i < bgiv.size(); i++){
				if(bgiv[i].find("NM:i:") != bgiv[i].npos){
					std::size_t ppos = 5;
					errnum = atoi((bgiv[i].substr(ppos, bgiv[i].size() - ppos)).c_str());
					break;
				}
			}
			
			float mmisrate = (float)errnum / (float)(bgiv9Len - ssum);
			if(mmisrate > GetUmplasmid.getMisrate() || 
				(ssum != 0 && (float)ssum / (float)bgiv9Len > (float)1 - GetUmplasmid.getMaprate())){
				bgiMap.insert(make_pair(bgiv[0],1));
				GetUmplasmid.setShortReadsNum(GetUmplasmid.getShortReadsNum() + 1);
				GetUmplasmid.setShortBasesNum(GetUmplasmid.getShortBasesNum() + bgiv9Len);	

				memset(bufFileFd, 0, FILEBUFSIZE);
				sprintf(bufFileFd, "@%s%s\n%s\n+\n%s\n", bgiv[0].c_str(), tag.c_str(), bgiv[9].c_str(), bgiv[10].c_str());
				write(fileFd, bufFileFd, strlen(bufFileFd));
			
			}
			
		}
		
	}
	samFileRead.close();

}


int GetUmplasmidprocess::doWork(const int argc, char **  argv ) 
{
    if(argc != 5){
        cout <<  " Usage: <maprate> <sam.list> <unmap> <stat>" << endl;
        return -1;
    }

	GetUmplasmid getUmplasmid;

    getUmplasmid.setMaprate(atof(argv[1]));
    getUmplasmid.setMisrate(atof(argv[2]));
    const char * samFile = argv[3];
    const char * umplaFile = argv[4];
     	
	int unmapFileFd = getUmplasmid.creatFile(umplaFile, true);
	if(0 > unmapFileFd) { perror(umplaFile);}
	
	filterStat(samFile, unmapFileFd, getUmplasmid);

	close(unmapFileFd);
	//#open STAT, ">$stat" or die $!;
	//print "unmapplasmid reads number: $total_um_rnum\nunmapplasmid reads base: $total_um_base\n";
	//print ("unmapplasmid reads number: %d\nunmapplasmid reads base: $total_um_base\n", $total_um_rnum);
	cout << "unmapplasmid reads number:" << getUmplasmid.getShortReadsNum() << "\n";
	cout << "unmapplasmid reads number:" << getUmplasmid.getShortBasesNum() << "\n";
    return 0;
}

