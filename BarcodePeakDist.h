//
// Created by gstoll on 01/07/25.
//

#ifndef BARCODEPEAKDIST_H
#define BARCODEPEAKDIST_H
#include "SinglePeakDist.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

class BarcodePeakDist : SinglePeakDist {
    const unsigned long bpStep; ///< base pair step for computing cumulative distribution
    const std::unordered_set<std::string>& barCodesSet; ///< set of cell names, predefined before adding peaks to the object
    std::unordered_map<std::string,std::vector<double>> densityUnnormProb; ///< unnormalized probability density, for each cells
    std::unordered_map<std::string,std::vector<double>> cumulUnnormProb; ///< unnormalized probability distribution, for each cells
    std::unordered_map<std::string,double> densityNormFactor; ///< normalization factor for density, should be set externally
    std::unordered_map<std::string,double> cumulNormFactor; ///< normalization factor for cumul, should be set externally

};



#endif //BARCODEPEAKDIST_H
