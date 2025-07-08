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
    std::string    barCodeFile = "/home/gstoll/CLionProjects/OpenChromDist/Test/smallBarCodes.txt";
    std::string    chromFile = "/home/gstoll/CLionProjects/OpenChromDist/Test/smallChromFile.txt";
    std::string fragFile = "/home/gstoll/CLionProjects/OpenChromDist/Test/smallFragFile.txt";

    PeakBasedDist pkBsDist = PeakBasedDist::fromFlatFile(chromFile,barCodeFile);
    std::cout << "Number for barCodes:" << pkBsDist.barCodeSet.size() << std::endl;
    std::cout << "Base pair Step:" << pkBsDist.bpStep << std::endl;
    std::cout << "Chromosome:" << pkBsDist.chromosome << std::endl;


    pkBsDist.addPeaksFromFragFile(fragFile,1000);
    std::cout << "Number of basCodes hits:" << pkBsDist.cumulUnnormProb.size() << std::endl;
                // pkBsDist.write2BinaryFile(outBindFile);

    for (const auto & [barCode,cumulProb] : pkBsDist.cumulUnnormProb) {;
     std::cout << barCode << std::endl;
     for (const auto & prob : cumulProb) {
         std::cout << prob << std::endl;
     }
    }
    return 0;
}