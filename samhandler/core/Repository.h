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
#include <tuple>

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
    string md;
} *psamopt;

typedef struct SamLine {
    string seqName;
    uint flag;
    string RefName;
    uint RefPos;
    uint mapQ;
    string cicarStr;
    string mateRefName;
    uint mateRefPos;
    int tLen;
    string readSeq;
    string readQual;
    SamOptField optField;
} *psamLine;

typedef struct SamSortIndex {
    uint index;
    uint endPos;

    std::tuple<uint, uint, uint, uint>  _key;

    /*uint chromosome;
    uint chrPos;
    uint mapQ;
    uint id;

    bool operator<=(const SamSortIndex& comp) {
        if (this->chromosome < comp.chromosome)
            return true;
        else if (this->chromosome = comp.chromosome) {
            if (this->chrPos < comp.chrPos)
                return true;
        }
        else
            return false;
    }*/
} *pSamSortIndex;

typedef std::vector<SamSortIndex>  sortIndecVec;

typedef struct StasticsData {
    uint sum;
    uint effectiveSum;
    uint xtU;
    uint xtR;
    uint minMapQ;
    uint maxMapQ;
    StasticsData(): sum(0),effectiveSum(0), xtU(0), xtR(0), minMapQ(0), maxMapQ(0) {}
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
