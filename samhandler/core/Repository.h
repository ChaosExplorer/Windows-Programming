//
// Created by chaos on 5/16/18.
//

#ifndef SAMHANDLER_REPOSITORY_H
#define SAMHANDLER_REPOSITORY_H

#include <mutex>
#include <condition_variable>

#include <string>
#include <map>
#include <vector>

#define READ_BUFF_SIZE 2500000000
#define PREDIC_ITEM_NUM 10000000
using std::string;

typedef struct SamHeader {
    string headerstr;
    uint size;
} *psamheader;

typedef struct SamOptField {
    char xt;
    int nm;
    int x0;
    int x1;
    int xm;
    int xo;
    int xg;
    char md[7];
} *psamopt;

typedef struct SamLine {
    char seqName[28];
    uint flag;
    char RefName[8];
    uint RefPos;
    uint mapQ;
    char cicarStr[8];
    char mateRefName[8];
    uint mateRefPos;
    int tLen;
    char readSeq[35];
    char readQual[35];
    SamOptField optField;
} *psamLine;

typedef struct SamSortIndex {
    uint index;
    uint endPos;

    uint chromosome;
    uint chrPos;
    uint mapQ;
    uint id;

    bool operator<=(const SamSortIndex& comp) {
        if (chromosome == comp.chromosome) {
            if (chrPos == comp.chrPos) {
                if (mapQ == comp.mapQ) {
                    if (id == comp.id)
                        return true;
                    else
                        return id < comp.id;
                }
                return mapQ < comp.mapQ;
            }
            return chrPos < comp.chrPos;
        }
        return chromosome < comp.chromosome;
    }
} *pSamSortIndex;

typedef std::vector<SamSortIndex>  sortIndecVec;

typedef struct StasticsData {
    uint sum;
    uint effectiveSum;
    uint xtU;
    uint xtR;
    uint minMapQ;
    uint maxMapQ;
    StasticsData(): sum(0),effectiveSum(0), xtU(0), xtR(0), minMapQ(40), maxMapQ(0) {}
} *pstastics;

class MapRepository {
    std::map<string, string> _buffMap;
    std::mutex _mutex;
public:
    MapRepository();
    void put(string& key, string& item);
    std::map<string, string>& get(uint num);
};

template <typename  T>
class VectorRepository {
    std::vector <T> _buffVec;
    std::mutex _mutex;
    std::condition_variable _not_full;
    std::condition_variable _not_empty;
public:
    VectorRepository ();
    void put (T& item);
};


#endif //SAMHANDLER_REPOSITORY_H
