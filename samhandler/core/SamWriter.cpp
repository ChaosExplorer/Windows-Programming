//
// Created by chaos on 5/16/18.
//

#include "SamWriter.h"

void Samwriter::writeAll(char *buffer, const sortIndecVec &indecVec, const off64_t header_endPos)
{
    // output header
    _out.write(buffer, header_endPos+1);

    // write body
    for (auto ite = indecVec.begin(); ite != indecVec.end(); ++ite) {
        _out.write(buffer + ite->index, ite->endPos - ite->index + 1);
    }
}

void Samwriter::writePart(char *buffer, uint size)
{

}

