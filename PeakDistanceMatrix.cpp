//
// Created by gstoll on 07/07/25.
//

#include "PeakDistanceMatrix.h"

#include <fstream>
#include <sstream>

PeakDistanceMatrix::PeakDistanceMatrix(const std::unordered_set<std::string> & bareCodeSet1,const std::unordered_set<std::string> & bareCodeSet2,
                                       const PeakBasedDist & pBasedDist1,const PeakBasedDist & pBasedDist2,const std::optional<std::string> & oPOutFile) :
barCodeVect1(bareCodeSet1.begin(),bareCodeSet1.end()),barCodeVect2(bareCodeSet2.begin(),bareCodeSet2.end()) {
    std::ofstream outFile;
    if (oPOutFile.has_value()) {
        outFile.open(oPOutFile.value());
        if (!outFile.is_open()) {throw std::runtime_error("cannot open file " + oPOutFile.value());}
        outFile << "#BarCode1\tBarCod2\tDistance" << std::endl;
    }
    if (pBasedDist1.bpStep != pBasedDist2.bpStep) {
        throw std::invalid_argument("Not equal base pair step " +
            std::to_string(pBasedDist1.bpStep) + " " + std::to_string(pBasedDist2.bpStep));
    }
    if (pBasedDist1.chrLength != pBasedDist2.chrLength) {
        throw std::invalid_argument("Not equal chromosome length"  +
            std::to_string(pBasedDist1.chrLength) + " " + std::to_string(pBasedDist2.chrLength));
    }

    if (!oPOutFile.has_value()){distanceFlatMatrix.reserve(bareCodeSet1.size()*(bareCodeSet2.size()));}
    for (std::string bareCode1:barCodeVect1) {
        auto itBareCode1 = pBasedDist1.cumulUnnormProb.find(bareCode1);
        if (itBareCode1 == pBasedDist1.cumulUnnormProb.end()) {throw std::invalid_argument(bareCode1 + " not found in first distribution");}
        double normFactorBC1 = pBasedDist1.normFactor.find(bareCode1)->second;
        if (normFactorBC1 == NAN) {normFactorBC1 = 1.0;}
        for (std::string bareCode2:barCodeVect2) {
            auto itBareCode2 = pBasedDist2.cumulUnnormProb.find(bareCode1);
            if (itBareCode2 == pBasedDist2.cumulUnnormProb.end()) {throw std::invalid_argument(bareCode2 + " not found in second distribution");}
            double normFactorBC2 = pBasedDist2.normFactor.find(bareCode2)->second;
            if (normFactorBC2 == NAN) {normFactorBC2 = 1.0;}
            double maxAbs = 0.0;
            for (size_t cumlProbIndex=0;cumlProbIndex< (itBareCode1 -> second).size();cumlProbIndex++) {
                maxAbs = std::max(maxAbs,std::abs((itBareCode1 -> second)[cumlProbIndex]/normFactorBC1 - (itBareCode2 -> second)[cumlProbIndex]/normFactorBC2));
            }
            if (oPOutFile.has_value()) {
             outFile << bareCode1 << "\t" << bareCode2 << "\t" << maxAbs << std::endl;
            } else {
                distanceFlatMatrix.push_back(maxAbs);
            }
        }
    }
outFile.close();

}

void PeakDistanceMatrix::writeMatrix() {
    std::cout << "nb of rows: " << this->barCodeVect1.size() << " nb of columns: " << this->barCodeVect2.size();
    std::cout << " flat matrix size " << this->distanceFlatMatrix.size() << std::endl;
    auto itFlatMat = this->distanceFlatMatrix.begin();
    for (std::string bC1:this->barCodeVect1) {
        for (std::string bC2:this->barCodeVect2) {
            std::cout << bC1 << "\t" << bC2 << "\t" << *itFlatMat << std::endl;
            ++itFlatMat;}}
}
