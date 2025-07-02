//
// Created by gstoll on 18/06/25.
//

#include "PeakBasedDist.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <optional>

void PeakBasedDist::addPeak(const std::string & cellBarCode, unsigned long position, unsigned int count, std::optional<unsigned long> windEval) {
    // add test for cellBarCode name
    if (barCodeSet.contains(cellBarCode)) {
        unsigned long windEvalUsed;
        if (windEval) {windEvalUsed = windEval.value();} else {windEvalUsed = chrLength;}

        cumulUnnormProb[cellBarCode][0] +=
                   static_cast<double>(count)*static_cast<double>(windSize) * std::numbers::pi / 2 *
                   (erf(static_cast<double>(bpStep - position)/static_cast<double>(windSize)) -
                       erf(static_cast<double>(-position)/static_cast<double>(windSize)));

        for (size_t indexBp = 1; (indexBp+1)*bpStep < chrLength; indexBp++ ) {
            if ( (std::abs(static_cast<long>((indexBp + 1) * bpStep - position)) < windEvalUsed)  | (std::abs(static_cast<long>(indexBp * bpStep - position)) < windEvalUsed)) {
                cumulUnnormProb[cellBarCode][indexBp] += cumulUnnormProb[cellBarCode][indexBp-1] +
                    static_cast<double>(count)*static_cast<double>(windSize) * std::numbers::pi / 2 *
                    (erf(static_cast<double>((indexBp+1)*bpStep-position)/static_cast<double>(windSize)) -
                        erf(static_cast<double>(indexBp*bpStep-position)/static_cast<double>(windSize)));
            }
        }
        cumulUnnormProb[cellBarCode][chrLength/bpStep] = static_cast<double>(count)*static_cast<double>(windSize) * std::numbers::pi / 2 *
            (erf(static_cast<double>(chrLength-position)/static_cast<double>(windSize)) -
                erf(static_cast<double>(chrLength-(chrLength % bpStep)-position)/static_cast<double>(windSize)));
    }
};

void PeakBasedDist::addPeaksFromFragFile(const std::string & fragFile,const unsigned long & windEval) {
    std::ifstream fragFStr(fragFile);
    if (!fragFStr.is_open()) throw std::runtime_error("Failed to open fragment file");
    std::string line;
    while (std::getline(fragFStr, line)) {
        if (!line.empty() && line[0] != '#') {
            std::stringstream lineStream(line);
            std::vector<std::string> tokenVector;
            std::string token;
            while (getline(lineStream, token,'\t')) {
                tokenVector.push_back(token);
            }
            if (tokenVector.size() >= 5) {
                if (tokenVector[0] == chromosome) {
                    addPeak(tokenVector[3],
                        (strToUnsLong(tokenVector[1])+strToUnsLong(tokenVector[2]))/2,
                        strToUnsLong(tokenVector[4]),
                        windEval);
            }
            } else {throw std::runtime_error("Bad line in fragment file "+line);}
        }
    }
};

void PeakBasedDist::write2BinaryFile(const std::string & binFile) {
    std::ofstream ofs(binFile, std::ios::binary);
    if (!ofs.is_open()) throw std::runtime_error("Failed to open binary file");

    uint32_t len = chromosome.size();
    ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
    ofs.write(chromosome.data(), len);

    ofs.write(reinterpret_cast<const char*>(&chromosome), sizeof(std::string));

    ofs.write(reinterpret_cast<const char*>(&chrLength), sizeof(unsigned long));

    ofs.write(reinterpret_cast<const char*>(&bpStep), sizeof(unsigned long));

    ofs.write(reinterpret_cast<const char*>(&windSize), sizeof(unsigned long));

    uint64_t set_size = barCodeSet.size();
    ofs.write(reinterpret_cast<const char*>(&set_size), sizeof(set_size));
    for (const std::string& str : barCodeSet) {
        //uint32_t len = str.size();
        len = str.size();
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ofs.write(str.data(), len);
    }

    uint64_t num_entries = cumulUnnormProb.size();
    ofs.write(reinterpret_cast<const char*>(&num_entries), sizeof(num_entries));

    for (const auto& [key, vec] : cumulUnnormProb) {
        uint32_t key_len = key.size();
        ofs.write(reinterpret_cast<const char*>(&key_len), sizeof(key_len));
        ofs.write(key.data(), key_len);

        uint64_t vec_len = vec.size();
        ofs.write(reinterpret_cast<const char*>(&vec_len), sizeof(vec_len));
        ofs.write(reinterpret_cast<const char*>(vec.data()), vec_len * sizeof(double));
    }
    // missing normalization factor
}

PeakBasedDist PeakBasedDist::fromBinFile(const std::string & binFile) {
    std::ifstream ifs(binFile, std::ios::binary);
    if (!ifs) throw std::runtime_error("Failed to open file");

    uint32_t len;
    ifs.read(reinterpret_cast<char*>(&len), sizeof(len));

    std::string chromosomeFromFile(len, '\0');
    ifs.read(&chromosomeFromFile[0], len);

    unsigned long chrLengthFromFile;
    ifs.read(reinterpret_cast<char*>(&chrLengthFromFile), sizeof(unsigned long));

    unsigned long bpStepFromFile;
    ifs.read(reinterpret_cast<char*>(&bpStepFromFile), sizeof(unsigned long));

    unsigned long windSizeFromFile;
    ifs.read(reinterpret_cast<char*>(&windSizeFromFile), sizeof(unsigned long));

    std::unordered_set<std::string> barCodeSetFromFile;
    uint64_t set_size;
    ifs.read(reinterpret_cast<char*>(&set_size), sizeof(set_size));

    for (uint64_t i = 0; i < set_size; ++i) {
        //uint32_t len;
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));

        std::string str(len, '\0');
        ifs.read(&str[0], len);

        barCodeSetFromFile.insert(std::move(str));
    }
    PeakBasedDist pkBaseDist = PeakBasedDist(chromosomeFromFile,chrLengthFromFile,bpStepFromFile,windSizeFromFile,barCodeSetFromFile);

    uint64_t num_entries;
    ifs.read(reinterpret_cast<char*>(&num_entries), sizeof(num_entries));
    for (uint64_t i = 0; i < num_entries; ++i) {
        uint32_t key_len;
        ifs.read(reinterpret_cast<char*>(&key_len), sizeof(key_len));
        std::string key(key_len, '\0');
        ifs.read(&key[0], key_len);

        uint64_t vec_len;
        ifs.read(reinterpret_cast<char*>(&vec_len), sizeof(vec_len));
        ifs.read(reinterpret_cast<char*>(pkBaseDist.cumulUnnormProb[key].data()), vec_len * sizeof(double));
    }
    //missing normalization factor
    return pkBaseDist;
};

PeakBasedDist PeakBasedDist::fromFlatFile(const std::string & chrFile,const std::string &barCodeFile) {
    std::unordered_map<std::string,std::string> chromDescr;
    std::unordered_set<std::string> barCodeSet;
    std::string line;
    std::ifstream chrFStr(chrFile);
    if (!chrFStr.is_open()) throw std::runtime_error("Failed to open chromosome file");
    while (std::getline(chrFStr, line)) {
        std::size_t sep_pos = line.find('=');
        if (sep_pos != std::string::npos) {
            std::string key = line.substr(0, sep_pos);
            std::string value = line.substr(sep_pos + 1);
            chromDescr[key] = value;
        }
    }
    chrFStr.close();
    std::ifstream barCFStr(barCodeFile);
    if (!barCFStr.is_open()) throw std::runtime_error("Failed to open barcode file");
    while (std::getline(barCFStr, line)) {
        barCodeSet.insert(line);
    }
    barCFStr.close();

    std::string chromosome;
    unsigned long chrLength;
    unsigned long bpStep;
    unsigned long windSize;

    auto findIt = chromDescr.find("CHROMOSOME");
    if (findIt != chromDescr.end()) {chromosome = findIt->second;} else {throw std::runtime_error("CHROMOSOME?");}

    findIt = chromDescr.find("CHRLENGTH");
    if (findIt != chromDescr.end()) {
            chrLength = strToUnsLong(findIt->second);
    } else {throw std::runtime_error("CHRLENGTH?");}

    findIt = chromDescr.find("BPSTEP");
    if (findIt != chromDescr.end()) {
            bpStep = strToUnsLong(findIt->second);
    } else {throw std::runtime_error("BPSTEP?");}

    findIt = chromDescr.find("WINDSIZE");
    if (findIt != chromDescr.end()) {
            windSize = strToUnsLong(findIt->second);
    } else {throw std::runtime_error("WINDSIZE?");}

    PeakBasedDist pkBaseDist = PeakBasedDist(chromosome,chrLength,bpStep,windSize,barCodeSet);
    std::cout << "Class created" << std::endl;
    return pkBaseDist;
};

unsigned long PeakBasedDist::strToUnsLong(const std::string & str) {
    unsigned long unLong;
    try {
        unLong = std::stol(str);
    }
    catch (const std::invalid_argument& e) { throw std::runtime_error("Failed to convert " + str); }
    catch (const std::out_of_range& e) { throw std::runtime_error("Out fo range " + str); }
    return unLong;
};
