//
// Created by gstoll on 18/06/25.
//

#ifndef PEAKBASEDDIST_H
#define PEAKBASEDDIST_H
#include <map>
#include <cmath>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @class PeakBasedDist
 * @brief probability distribution based on peaks
 * Each peak defines a gaussian probability density of a given sd set by a window size
 * An unnormalized cumulative probability distribution is computed for each cell (defined by a barcode)
 * The set of values for the unnormalized cumulative probability distribution is given for each bpStep (+ the length of the chromosome)
 */
class PeakBasedDist { //many cells, a unique chromosome
public:
    const std::string chromosome; ///< name of chromosome
    const unsigned long chrLength; ///< length of chromosome
    const unsigned long bpStep; ///< base pair step for computing cumulative distribution
    const unsigned long windSize; ///< sd of gaussian probability density of each peak
    const std::unordered_set<std::string> barCodeSet; ///< set of cell names, predefined before adding peaks to the object
    std::unordered_map<std::string,std::vector<double>> cumulUnnormProb; ///< unnormalized probability distribution, for each cells
    std::unordered_map<std::string,double> normFactor; ///< normalization factor, should be set externally
    /**
     * @brief constructor of empty PeakBasedDist
     * @param chromosome chromosome name
     * @param chrLength chromosome length
     * @param bpStep evaluation point of unnormalized cumulative probability distribution
     * @param windSize sd of gaussion probability density for each peak
     * @param barCodeSet names of cell, a priori definition (could be based on external QC for instance)
     */
    explicit PeakBasedDist(std::string & chromosome, unsigned long chrLength,unsigned long bpStep,unsigned long windSize, const std::unordered_set<std::string> & barCodeSet) :
    chromosome(chromosome), chrLength(chrLength),bpStep(bpStep),windSize(windSize),barCodeSet(barCodeSet) {
        for (const std::string & barCode : barCodeSet) { cumulUnnormProb[barCode]= std::vector<double>((chrLength/bpStep+1),0);
            normFactor[barCode] = NAN; };
    }

    /**
     * @brief static constructor, using binary file (missing norm factor)
     * @return a PeakBasedDist object
     */
    static PeakBasedDist fromBinFile(const std::string &);

    /**
     * @brief construct from flat files
     * @param chrFile chromosome description
     *  CHROMOSOME=
     *  CHRLENGTH=
     *  BPSTEP=
     *  WINDSIZE=
     * @param barCodeFile list of barcode separated by \n
     * @return an empty PeakBasedDist object
     */
    static PeakBasedDist fromFlatFile(const std::string & chrFile,const std::string &barCodeFile);

    /**
     * @brief update object, given a new peak, in particular the cumulative unnormalized probability distribution
     * @param cellBarCode cell name. If it is not an element of barCodeSet, nothing is done
     * @param position peak position
     * @param count
     * @param windEval window around peak inside which the density is supposed to be non-zero (approximation parameter for accelerating the distribution update)
     */
    void addPeak(const std::string & cellBarCode,unsigned long position,unsigned int count, std::optional<unsigned long> windEval);

    /**
     * @brief use a fragment tsv file that has chormosome, begin, end, barcode, count
     * @param fragFile
     * @param windEval window around peak inside which the density is supposed to be non-zero (approximation parameter for accelerating the distribution update)
     */
    void addPeaksFromFragFile(const std::string & fragFile ,const unsigned long & windEval);

    /**
     * @brief set normalization factor as 1/(cumul probability over the whole chromosome)
     *
     */
    void chrNormalize() {
        for (const std::string & barCode : barCodeSet) {
            normFactor[barCode] = 1.0/((this->cumulUnnormProb)[barCode].back());
        }
    };

    /**
     * @brief write object to binary file (missing norm factor)
     * binary format is very compact, can only be read within fromBinFile static method
     * @param binFile
     */
    void write2BinaryFile(const std::string & binFile);

    static unsigned long strToUnsLong(const std::string &);
};

#endif //PEAKBASEDDIST_H
