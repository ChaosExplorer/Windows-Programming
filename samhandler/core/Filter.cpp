//
// Created by chaos on 5/16/18.
//

#include <stdio.h>
#include <cstring>
#include "Filter.h"
#include "Repository.h"

const static char mandoryRule[] = "%s\t%d\t%s\t%d\t%d\t%s\t%s\t%d\t%d\t%s\t%s\t";
const static char optionRule[] = "%[^:]:%*[^:]:%[^:]";


void Filter::goForAll(char *buffer, sortIndecVec &indexVec, StasticsData &stat) {
    off64_t header_end_pos = 0;
    off64_t record_head = 0;
    off64_t record_tail = 0;
    
    char* tag;

    // findHead
    while ((tag = strchr(&buffer[record_head],'\n')) != NULL) {
        if (buffer[record_head] != '@') {
            record_head = tag - buffer + 1;
            break;
        }
        record_head = tag - buffer + 1;
    }
    header_end_pos = record_head - 1;
    
    // analyze body
    char record[128];
    SamSortIndex index;
    SamLine samline;

    while ((tag = strchr(&buffer[record_head],'\n')) != NULL) {
        ++stat.sum;
        
        record_tail = tag - buffer;
        
        strncpy(record, &buffer[record_head], record_tail - record_head + 1);

        //memset(&samline, 0, sizeof(SamLine));
        samline = (const SamLine){0};
        if (checkRecord(record, index, samline)) {
            indexVec.push_back(index);
            ++stat.effectiveSum;
        }
        
        record_head = record_tail + 2;
    }
}

bool Filter::checkRecord(char* record, SamSortIndex& index, SamLine& line)
{
    sscanf(record, mandoryRule, line.seqName, &line.flag, line.RefName, \
          &line.RefPos, &line.mapQ, line.cicarStr, line.mateRefName, &line.mateRefPos, \
          &line.tLen, line.readSeq, line.readQual);

    if (line.mapQ < _cond.mapQ)
        return false;

    char *optionstart;
    if ((optionstart = strchr(record, ':')) == NULL)
        return false;

    optionstart -= 2;    // to the first option

    char* opt;
    char optName[3], key[7];
    opt = strtok(optionstart, "\t");
    while (opt != NULL) {
        if (strlen(opt) > 0) {
            sscanf(opt, optionRule, optName, key);
            if (strcmp(optName, "XT") == 0)
                line.optField.xt = *key;
            else if (strcmp(optName, "NM") == 0)
                line.optField.xt = atoi(key);
            else if (strcmp(optName, "X0") == 0)
                line.optField.x0 = atoi(key);
            else if (strcmp(optName, "X1") == 0)
                line.optField.x1 = atoi(key);
            else if (strcmp(optName, "XM") == 0)
                line.optField.xm = atoi(key);
            else if (strcmp(optName, "XO") == 0)
                line.optField.xo = atoi(key);
            else if (strcmp(optName, "XG") == 0)
                line.optField.xg = atoi(key);
            else if (strcmp(optName, "MD") == 0)
                strcpy(line.optField.md, key);
        }

        opt = strtok(NULL, "\t");    // continue previous
    }
}
