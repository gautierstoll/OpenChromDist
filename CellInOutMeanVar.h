//
// Created by gstoll on 17/11/2025.
//

#ifndef OPENCHROMDIST_CELLINOUTMEANVAR_H
#define OPENCHROMDIST_CELLINOUTMEANVAR_H
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>

/**
 * @brief state given by mean and var of fragment position
 */
class CellInOutMeanVar {
public:
    unsigned long nbPeak = 0; ///< number of peaks
    double mean = 0.0; ///< estimated mean
    double var = 0.0; ///< estimated var
    /**
     * @brief update function based on single fragment line
     * @param line
     */
    void update(const std::string & line) {
        std::stringstream lineStream(line);
        std::vector<std::string> tokenVector;
        std::string token;
        while (getline(lineStream, token,'\t')) {
            tokenVector.push_back(token);
        }
        if (tokenVector.size() >= 5) {
            cumulNbPeak+= std::stoul(tokenVector[4]);
            cumulPos+= std::stod(tokenVector[4])*(std::stod(tokenVector[1])+std::stod(tokenVector[2]))/2;
            cumulSqPos += std::stod(tokenVector[4])*
        (std::stod(tokenVector[1])+std::stod(tokenVector[2]))*(std::stod(tokenVector[1])+std::stod(tokenVector[2]))/4;
        } else {throw std::runtime_error("Bad line: "+line);};
    }

    /**
     * @brief compute mean/var for cumulative values
     */
    void epilogue() {
        nbPeak = cumulNbPeak;
       mean = cumulPos/static_cast<double>(nbPeak);
        var = (cumulSqPos/static_cast<double>(nbPeak) - mean * mean)*
            (static_cast<double>(nbPeak)/static_cast<double>(nbPeak-1));
    }

    /**
     *
     * @return output string of values separated by tab
     */
    [[nodiscard]] std::string toString() const {
    return(std::to_string(nbPeak)+"\t"+
        std::to_string(mean)+"\t"+
        std::to_string(var)+"\t");
    }
private:
    unsigned long cumulNbPeak = 0; ///< cumulative nub of peaks
     double cumulPos = 0;///< cumulative peak positions
    double cumulSqPos = 0;///< cumulative square of peak positions
};


#endif //OPENCHROMDIST_CELLINOUTMEANVAR_H