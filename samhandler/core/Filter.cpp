//
// Created by chaos on 5/16/18.
//

#include <stdio.h>
#include <cstring>
#include <iostream>
#include "Filter.h"
#include "Repository.h"

const static char mandoryRule[] = "%s\t%d\t%s\t%d\t%d\t%s\t%s\t%d\t%d\t%s\t%s\t";
const static char optionRule[] = "%[^:]:%*[^:]:%[^:]";
const static uint chromosomeRule = 3;


void Filter::goForAll(char *buffer, sortIndecVec &indexVec, StasticsData &stat, off64_t &header_endPos) {
    off64_t record_head = 0;
    off64_t record_tail = 0;
    
    char* tag;

    // findHead
    while ((tag = strchr(buffer+record_head,'\n')) != NULL) {
        if (buffer[record_head] != '@') {
            break;
        }
        record_head = tag - buffer + 1;
    }
    header_endPos = record_head - 1;
    
    // analyze body
    char *record;
    size_t recordSize = 512;
    record = (char*)malloc(recordSize* sizeof(char));
    SamSortIndex index;
    SamLine samline;

    indexVec.reserve(PREDIC_ITEM_NUM);
    while ((tag = strchr(buffer+record_head,'\n')) != NULL) {
        ++stat.sum;

        record_tail = tag - buffer;

        memset(record, 0, recordSize*sizeof(char));

        if(record_tail - record_head > recordSize) {
            recordSize = record_tail - record_head;

            record = (char*)realloc(record, recordSize* sizeof(char));
        }

        memcpy(record, buffer + record_head, record_tail - record_head); // important_smashing
        //record[record_tail - record_head] = '\0';       // null terminator, preserve old data

        memset(&samline, 0, sizeof(SamLine));
        samline = (const SamLine){0};
        index = (const SamSortIndex){0};
        if (checkRecord(record, index, samline)) {
            index.index = record_head;
            index.endPos = record_tail;

            indexVec.push_back(index);

            // statistic
            ++stat.effectiveSum;
            if (samline.mapQ < stat.minMapQ)
                stat.minMapQ = samline.mapQ;
            if (samline.mapQ > stat.maxMapQ)
                stat.maxMapQ = samline.mapQ;
        }
        // statistic
        if (samline.optField.xt == 'U')
            ++stat.xtU;
        else if (samline.optField.xt == 'R')
            ++stat.xtR;


        record_head = record_tail + 1;
        }

    if(record){
        free(record);
    }

    //indexVec.shrink_to_fit(); // this O(n) will copy elements to theri new, smaller home(reallocation)
}

bool Filter::checkRecord(char* record, SamSortIndex& index, SamLine& line)
{
    sscanf(record, mandoryRule, line.seqName, &line.flag, line.RefName, \
          &line.RefPos, &line.mapQ, line.cicarStr, line.mateRefName, &line.mateRefPos, \
          &line.tLen, line.readSeq, line.readQual);

    if (line.mapQ < _cond.mapQ)
        return false;

    char *optionstart;
    if ((optionstart = strstr(record, "XT:")) == NULL)
        return false;

    char* opt;
    char optName[3], key[7];
    opt = strtok(optionstart, "\t");
    while (opt != NULL) {
        if (strlen(opt) > 0) {
            sscanf(opt, optionRule, optName, key);
            if (strcmp(optName, "XT") == 0)
                line.optField.xt = *key;
            else if (strcmp(optName, "NM") == 0)
                line.optField.nm = atoi(key);
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

    if (line.optField.xt != _cond.xt || line.optField.nm > _cond.nm)
        return false;

    // chr
    int chromosome = atoi(&line.RefName[chromosomeRule]);
    if (chromosome == 0) {
        chromosome = (int)line.RefName[chromosomeRule];
    }

    // sequence id
    long int seqId = 0;
    strtok(line.seqName, "_");
    opt = strtok(NULL, "_");
    if ((seqId = strtol(opt, NULL, 10)) == 0)
        seqId = (long int) *opt;

    index.chromosome = chromosome;
    index.chrPos = line.RefPos;
    index.mapQ = line.mapQ;
    index.id = seqId;
    return true;
}
