//
// Created by gstoll on 07/07/25.
//

#ifndef PEAKDISTANCEMATRIX_H
#define PEAKDISTANCEMATRIX_H
#include <string>
#include <unordered_set>
#include <vector>
#include <iostream>

#include "PeakBasedDist.h"

/**
 * @class PeakDistanceMatrix
 * @brief estimate Kolmogorov-Smirnov distance given the cumulative probability distribution
 * @brief computed in constructor, either keeping the flat matrix in memory or writing to a .csv file
 * @warning list of barcodes should be in PeakBasedDist inputs
 * @warning cumulative probability distribution should be normalized before
 */
class PeakDistanceMatrix {
    // using vector so they can defines the indices
public :
    const std::vector<std::string> barCodeVect1; ///< vector of first barCodes
    const std::vector<std::string> barCodeVect2; ///< vector of second barCodes
    std::vector<double> distanceFlatMatrix; ///< flat matrix of barCodes distances, filled by outer-loop over first barCode vector and inner-loop over second barCode vector
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

    /**
     * @brief Print the distance matrix in flat format
     */
    void writeMatrix() {
        std::cout << "nb of rows: " << this->barCodeVect1.size() << " nb of columns: " << this->barCodeVect2.size();
        std::cout << " flat matrix size " << this->distanceFlatMatrix.size() << std::endl;
        auto itFlatMat = this->distanceFlatMatrix.begin();
        for (const std::string & bC1:this->barCodeVect1) {
            for (const std::string & bC2:this->barCodeVect2) {
                std::cout << bC1 << "\t" << bC2 << "\t" << *itFlatMat << std::endl;
                ++itFlatMat;}}
    }


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
