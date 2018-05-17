//
// Created by chaos on 5/16/18.
//

#include <cstdio>
#include "Filter.h"
#include "Repository.h"

void Filter::goForAll(char *buffer, sortIndecVec &indecVec, StasticsData &stat) {
    off64_t header_end_pos = 0;
    off64_t record_head = 0;
    off64_t record_tail = 0;
    
    char* tag;
    while ((tag = strchr(&buffer[record_head]),'\n') != NULL) {
        if (buffer[record_head] != '@') {
            record_head = tag - buffer + 1;
            break;
        }
        record_head = tag - buffer + 1;
    }
    header_end_pos = record_head - 1;
    
    char record[128];
    SamSortIndex index;
    SamLine samline;
    while ((tag = strchr(&buffer[record_head]),'\n') != NULL) {
        ++stat.sum;
        
        record_end = tag - buffer - 1;
        
        strncpy(record, &buffer[record_head], record_end - record_head + 1);
        if (checkRecord(record, index)) {
            indexVec.push_back(index);
            ++stat.effectiveSum;
        }
        
        record_head = record_end + 2;   
    }
}

bool Filter::checkRecord(char* record, SamSortIndex& index, SamLine& line)
{
    sscanf(record,"%s%d%s%d%d%s%s%d%d%s%s", line.seqName, line.flag, line.RefName, \
          line.RefPos, line.mapQ, line.cicarStr, line.mateRefName, line.mateRefPos, \
          line.tLen, line.readSeq, line.readQual);
}
