//
// Created by gstoll on 27/06/25.
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "PeakBasedDist.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main(int argc, char* argv[]) {
    std::string    barCodeFile = "/home/gstoll/CLionProjects/OpenChromDist/Test/First5barCodes.txt";
    std::string    chromFile = "/home/gstoll/CLionProjects/OpenChromDist/Test/chromFile.txt";
    std::string fragFile = "/home/gstoll/CLionProjects/OpenChromDist/Test/FragATAC_LateRec_127p_chr1_First5barCodes.tsv";

    PeakBasedDist pkBsDist = PeakBasedDist::fromFlatFile(chromFile,barCodeFile);
    std::cout << "Number for barCodes:" << pkBsDist.barCodeSet.size() << std::endl;
    std::cout << "Base pair Step:" << pkBsDist.bpStep << std::endl;
    std::cout << "Chromosome:" << pkBsDist.chromosome << std::endl;


    pkBsDist.addPeaksFromFragFile(fragFile,1000);
    std::cout << "Number of basCodes hits:" << pkBsDist.cumulUnnormProb.size() << std::endl;

    for (const auto & [barCode,cumulProb] : pkBsDist.cumulUnnormProb) {;
     std::cout << barCode << std::endl;
     for (const auto & prob : cumulProb) {
          std::cout << prob << "\t";
     }
        std::cout << std::endl;
    }

    std::cout << "chormosome normalization" << std::endl;
    pkBsDist.chrNormalize();
    for (const auto & [barCode,cumulProb] : pkBsDist.cumulUnnormProb) {;
        std::cout << barCode << std::endl;
        for (const auto & prob : cumulProb) {
            std::cout << (prob/pkBsDist.normFactor.find(barCode)->second) << "\t";
        }
        std::cout << std::endl;
    }



    pkBsDist.write2BinaryFile("/home/gstoll/CLionProjects/OpenChromDist/Test/smallFrag.bin");
    std::cout << "Wrote binary file" << std::endl;


    PeakBasedDist pkBsDistRFromFile =
        PeakBasedDist::fromBinFile("/home/gstoll/CLionProjects/OpenChromDist/Test/smallFrag.bin");
    std::cout << "Read binary file" << std::endl;


    std::cout << "Number of basCodes hits:" << pkBsDistRFromFile.cumulUnnormProb.size() << std::endl;
    // pkBsDist.write2BinaryFile(outBindFile);

    for (const auto & [barCode,cumulProb] : pkBsDistRFromFile.cumulUnnormProb) {;
        std::cout << barCode << std::endl;
        for (const auto & prob : cumulProb) {
            // std::cout << prob << std::endl;
        }
    }

    return 0;
}