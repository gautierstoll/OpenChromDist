//
// Created by gstoll on 01/07/25.
//

#ifndef SINGLEPEAKDIST_H
#define SINGLEPEAKDIST_H
#include <vector>


class SinglePeakDist {
public:
    unsigned long bpStep;
    virtual void addPeakToDensity (std::vector<double> & densityProb,
        unsigned long & position,
        const unsigned long & count);
    virtual void addPeakToCumul (std::vector<double> & cumulProb,
        const unsigned long & position,
        const unsigned long & counts);
};



#endif //SINGLEPEAKDIST_H
