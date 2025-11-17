//
// Created by gstoll on 17/11/2025.
//

#ifndef OPENCHROMDIST_CELLDATAMEANVAR_H
#define OPENCHROMDIST_CELLDATAMEANVAR_H
#include <regex>

#include "CellData.h"
#include "CellInOutMeanVar.h"


class CellDataMeanVar : public CellData<CellInOutMeanVar> {
public:
    explicit CellDataMeanVar(const std::vector<std::string> & barCodes) : CellData<CellInOutMeanVar>(barCodes) {}
    static CellDataMeanVar fromFlatFile(const std::string & barCodeFile) {
        return(CellDataMeanVar(CellData<CellInOutMeanVar>::BarCodesFromFlatFile(barCodeFile)));}
};

#endif //OPENCHROMDIST_CELLDATAMEANVAR_H