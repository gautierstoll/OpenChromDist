//
// Created by gstoll on 17/11/2025.
//

#ifndef OPENCHROMDIST_CELLDATAMEANVAR_H
#define OPENCHROMDIST_CELLDATAMEANVAR_H
#include <regex>

#include "CellData.h"
#include "CellInOutMeanVar.h"

/**
 * Concrete class of CellData for mean and variance of peak positions
 */
class CellDataMeanVar : public CellData<CellInOutMeanVar> {
public:
    explicit CellDataMeanVar(const std::vector<std::string> & barCodes) : CellData<CellInOutMeanVar>(barCodes) {}

    /**
     * @brief sonstructor from file of barcodes
     * @param barCodeFile
     * @return
     */
    static CellDataMeanVar fromFlatFile(const std::string & barCodeFile) {
        return(CellDataMeanVar(CellData<CellInOutMeanVar>::BarCodesFromFlatFile(barCodeFile)));}
};

#endif //OPENCHROMDIST_CELLDATAMEANVAR_H