//
// Created by gstoll on 14/11/2025.
//

#ifndef OPENCHROMDIST_CELLDATA_H
#define OPENCHROMDIST_CELLDATA_H
#include <string>
#include <concepts>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmph.h>
#include <cstring>

#endif //OPENCHROMDIST_CELLDATA_H

/**
 *@brief concept descibing a cell state
 */
template<typename C>
concept CellInOut = requires(C c,const std::string s) {
    { c.update(s) } -> std::same_as<void>;
    {c.epilogue()} -> std::same_as<void>;
    {c.toString()} -> std::same_as<std::string>;
};

/**
 * @brief abstract classe of cell data
 * @tparam Cell cell state
 */
template<CellInOut Cell>
class CellData {
public:
    /**
     * @brief contruct an emply vector of cells
     * @param barCodes list of barcodes tha idenfy cells
     */
    explicit CellData(const std::vector<std::string> & barCodes) : vectData(barCodes.size()), barCodes(barCodes) {
        barCodesChar.reserve(barCodes.size());
        for (const auto& k : barCodes) {barCodesChar.push_back(const_cast<char*>(k.c_str()));}
    }

    static std::vector<std::string> BarCodesFromFlatFile(const std::string &barCodeFile) {
        std::ifstream barCodeFStr(barCodeFile);
        std::vector<std::string> barCodes;
        barCodes.reserve(10000);
        std::string barCode;
        while (barCodeFStr >> barCode) barCodes.push_back(std::move(barCode));
        return(barCodes);
    }

    const std::vector<std::string> & barCodes;

    /**
     * @brief read fragment files
     * @param fragFile
     */
    void readFragFile(const std::string & fragFile) {
    std::ifstream fragFStr(fragFile);
    if (!fragFStr.is_open()) throw std::runtime_error("Failed to open fragment file");
    std::string line;
    size_t  fileLine = 0;
    while (std::getline(fragFStr, line)) {
        fileLine++;
        if (!line.empty() && line[0] != '#') {
            std::stringstream lineStream(line);
            std::vector<std::string> tokenVector;
            std::string token;
            while (getline(lineStream, token,'\t')) {tokenVector.push_back(token);}
            if (tokenVector.size() >= 5) {
                std::cout << "\r File Line: " << fileLine <<
                        " Add bareCode: " << tokenVector[3] << " at position " << tokenVector[1] << std::flush;
                int index = cmph_search(hashBarCodes,tokenVector[3].c_str(),static_cast<cmph_uint32>(tokenVector[3].size()));
                vectData[index].update(line);
            } else {throw std::runtime_error("Bad line in fragment file "+line);}
        }
    }
    std::cout << std::endl;
}
    /**
     * @brief write cell states to cout
     */
    void toCout() {
        for (size_t i =0; i < barCodes.size() ; i++) {std::cout << barCodes[i] << "\t" <<  vectData[i].toString() << "\n";}
    }

protected:
    std::vector<Cell> vectData;
    std::vector<char *> barCodesChar;
    cmph_io_adapter_t *source = cmph_io_vector_adapter(barCodesChar.data(),barCodes.size());
    cmph_config_t *config = cmph_config_new(source);
    cmph_t *hashBarCodes = cmph_new(config);
};

