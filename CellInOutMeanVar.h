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

class CellInOutMeanVar {
public:
    unsigned long nbPeak = 0;
    double mean = 0.0;
    double var = 0.0;
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
    void epilogue() {
        nbPeak = cumulNbPeak;
       mean = cumulPos/static_cast<double>(nbPeak);
        var = cumulSqPos/static_cast<double>(nbPeak) - mean * mean;
    }
[[nodiscard]] std::string toString() const {
    return(std::to_string(nbPeak)+"\t"+
        std::to_string(mean)+"\t"+
        std::to_string(var)+"\t");
    }
private:
    unsigned long cumulNbPeak = 0;
     double cumulPos = 0;
    double cumulSqPos = 0;
};


#endif //OPENCHROMDIST_CELLINOUTMEANVAR_H