//
// Created by Pason on 2019-7-1.
//

#include "GetUmplasmid.h"

#define FILEBUFSIZE 2048

using namespace std;

class GetUmplasmidprocess{

public:
	void filterStat(const char * samfileName, const int fileFd, GetUmplasmid & getUmplasmid);
	int doWork(const int argc, char **argv );
};
