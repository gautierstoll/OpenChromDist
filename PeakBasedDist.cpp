//
// Created by gstoll on 18/06/25.
//

#include "PeakBasedDist.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <optional>

void PeakBasedDist::addPeak(const std::string & cellBarCode, unsigned long position, unsigned int count, std::optional<unsigned long> windEval) {
    // add test for cellBarCode name
    if (barCodesSet.contains(cellBarCode)) {
        unsigned long windEvalUsed;
        if (windEval) {windEvalUsed = windEval.value();} else {windEvalUsed = chrLength;}

        cumulUnnormProb[cellBarCode][0] +=
                   count*windSize * std::numbers::pi / 2 *
                   (erf(static_cast<double>(bpStep - position)/windSize) -
                       erf(static_cast<double>(-position)/windSize));

        for (size_t indexBp = 1; (indexBp+1)*bpStep < chrLength; indexBp++ ) {
            if ( (std::abs(static_cast<long>((indexBp + 1) * bpStep - position)) < windEvalUsed)  | (std::abs(static_cast<long>(indexBp * bpStep - position)) < windEvalUsed)) {
                cumulUnnormProb[cellBarCode][indexBp] += cumulUnnormProb[cellBarCode][indexBp-1] +
                    count*windSize * std::numbers::pi / 2 *
                    (erf(static_cast<double>((indexBp+1)*bpStep-position)/windSize) -
                        erf(static_cast<double>(indexBp*bpStep-position)/windSize));
            }
        }
        cumulUnnormProb[cellBarCode][chrLength/bpStep] = count*windSize * std::numbers::pi / 2 *
            (erf(static_cast<double>(chrLength-position)/windSize) -
                erf(static_cast<double>(chrLength-(chrLength % bpStep)-position)/windSize));
    }
};

void PeakBasedDist::write2BinaryFile(const std::string & binFile) {
    std::ofstream ofs(binFile, std::ios::binary);
    if (!ofs) throw std::runtime_error("Failed to open file");

    uint32_t len = chromosome.size();
    ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
    ofs.write(chromosome.data(), len);

    ofs.write(reinterpret_cast<const char*>(&chromosome), sizeof(std::string));

    ofs.write(reinterpret_cast<const char*>(&chrLength), sizeof(unsigned long));

    ofs.write(reinterpret_cast<const char*>(&bpStep), sizeof(unsigned long));

    ofs.write(reinterpret_cast<const char*>(&windSize), sizeof(unsigned long));

    uint64_t set_size = barCodesSet.size();
    ofs.write(reinterpret_cast<const char*>(&set_size), sizeof(set_size));
    for (const std::string& str : barCodesSet) {
        uint32_t len = str.size();
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
        uint32_t len;
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));

        std::string str(len, '\0');
        ifs.read(&str[0], len);

        barCodeSetFromFile.insert(std::move(str));
    }
    PeakBasedDist pkBaseDist = PeakBasedDist(chromosomeFromFile,chrLengthFromFile,windSizeFromFile,bpStepFromFile,barCodeSetFromFile);

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
