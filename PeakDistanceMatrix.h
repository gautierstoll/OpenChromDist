//
// Created by gstoll on 07/07/25.
//

#ifndef PEAKDISTANCEMATRIX_H
#define PEAKDISTANCEMATRIX_H
#include <string>
#include <unordered_set>
#include <vector>

#include "PeakBasedDist.h"

/**
 * @class PeakDistanceMatrix
 * @brief compute kolmogorov-smirnov distance given the cumulative probability distribution
 * @brief computed in constructor, either keeping the flat matrix in memory or wriring to a .csv file
 * @warning list of barcodes should be in Peak Based distribution
 * @warning cumulative probability distribution should be normalized before
 */
class PeakDistanceMatrix {
    // using vector so they can defines the indices
public :
    std::vector<std::string> barCodeVect1; ///< vector of first barCodes
    std::vector<std::string> barCodeVect2; ///< vector of second barCodes
    std::vector<double> distanceFlatMatrix; ///< flat matrix of barCodes distrances, filles by outer-loop over first vector and inner-loop over second vector
    /**
     * @brief construct vector of barcodes and flat matrix
     * @param bareCodeSet1 First list of bareCodes
     * @param bareCodeSet2 Second list of barCodes
     * @param pBasedDist1 First Peak Based Distribution
     * @param pBasedDist2 Second Peak Based Distribution
     */
    explicit PeakDistanceMatrix(const std::unordered_set<std::string> & bareCodeSet1,const std::unordered_set<std::string> & bareCodeSet2,
                                const PeakBasedDist & pBasedDist1,const PeakBasedDist & pBasedDist2) :
    PeakDistanceMatrix(bareCodeSet1,bareCodeSet2,pBasedDist1,pBasedDist2,std::nullopt){}

    /**
     * @brief For distances between the same dataset
     * @param bareCodeSet list of bareCodes
     * @param pBasedDist Peak Based Distribution
     */
    explicit PeakDistanceMatrix(const std::unordered_set<std::string> & bareCodeSet,const PeakBasedDist & pBasedDist) :
    PeakDistanceMatrix(bareCodeSet,bareCodeSet,pBasedDist,pBasedDist) {}

    /**
     * @brief construct vector of barcodes, print flat matrix in .csv file
     * @param bareCodeSet1 First list of bareCodes
     * @param bareCodeSet2 Second list of barCodes
     * @param pBasedDist1 First Peak Based Distribution
     * @param pBasedDist2 Second Peak Based Distribution
     * @param outFile output .csv file
     */
    explicit PeakDistanceMatrix(const std::unordered_set<std::string> & bareCodeSet1,const std::unordered_set<std::string> & bareCodeSet2,
                                const PeakBasedDist & pBasedDist1,const PeakBasedDist & pBasedDist2,const std::string & outFile) :
    PeakDistanceMatrix(bareCodeSet1,bareCodeSet2,pBasedDist1,pBasedDist2,std::make_optional(outFile)) {}

    /**
     * @brief For distances between the same dataset
     * @param bareCodeSet list of bareCodes
     * @param pBasedDist Peak Based Distribution
     * @param outFile output .csv file
     */
    explicit PeakDistanceMatrix(const std::unordered_set<std::string> & bareCodeSet,const PeakBasedDist & pBasedDist,const std::string & outFile) :
    PeakDistanceMatrix(bareCodeSet,bareCodeSet,pBasedDist,pBasedDist, outFile) {}

private:
    /**
     * @brief inner constructor with option over output file
     * @param bareCodeSet1
     * @param bareCodeSet2
     * @param pBasedDist1
     * @param pBasedDist2
     * @param oPOutFile
     */
    explicit PeakDistanceMatrix(const std::unordered_set<std::string> & bareCodeSet1,const std::unordered_set<std::string> & bareCodeSet2,
                                const PeakBasedDist & pBasedDist1,const PeakBasedDist & pBasedDist2,const std::optional<std::string> & oPOutFile);

};

#endif //PEAKDISTANCEMATRIX_H
