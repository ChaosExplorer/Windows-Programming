//
// Created by chaos on 5/16/18.
//

#include <cstdio>
#include "Filter.h"
#include "Repository.h"

void Filter::goForAll(char *buffer, sortIndecVec &indecVec, StasticsData &stat) {
    off64_t header_end_pos = 0;
    off64_t body_start_pos = 0;
    off64_t record_head = 0;
    off64_t record_tail = 0;

    off_t find_start_pos = 0;

    char* tag;
    while ((tag = strchr(&buffer[find_start_pos]),'\n') != NULL) {
        find_start_pos = tag - buffer + 1;
        if (buffer[find_start_pos] != '@')
            break;
    }
    header_end_pos = find_start_pos - 1;
    
    while ((tag = strchr(&buffer[find_start_pos]),'\n') != NULL) {
        find_start_pos = tag - buffer + 1;
        
    }
}
