#include <fstream>
#include <iostream>
#include <sstream>
#include <sstream>
#include <vector>

#include "PeakBasedDist.h"
#include "PeakDistanceMatrix.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main(int argc, char* argv[]) {
    std::ostringstream helpStrStr;
    helpStrStr << "For creating distribution:"
    << "  OpenChromDist chromFile barCodeFile fragFile windEval outBindFile\n"
    << "    chromFile contains\n      CHROMOSOME=\n      CHRLENGTH=\n      BPSTEP=\n      WINDSIZE=\n"
    << "    barCodeFile contains a list of barcode separated by newline\n"
    << "    with no output binary file, distance matrix is computed\n\n"
    << "For computing distance matrix from binary file of distribution: \n"
    <<  "  OpenChromDist -l binFile\n\n"
    << "For help:\n"
    << "  OpenChromDist -h\n"
    << std::endl;

    if (argc > 1) {
        std::string argv1 = argv[1];
        if (argv1 == "-h") {
            std::cout << helpStrStr.str();
            return 0;}
        if (argv1 == "-l") {
            if (argc > 2) {
                std::string binFile = argv[2];
                PeakBasedDist pkBsDist = PeakBasedDist::fromBinFile(binFile);
                pkBsDist.chrNormalize();
                PeakDistanceMatrix pkDistanceMatrix(pkBsDist.barCodeSet,pkBsDist);
                pkDistanceMatrix.writeMatrix();
                return 0;}
            std::cerr << "No binary file\n\n" << helpStrStr.str();
            return 1;}
        if (argc > 4)
        {
            std::string chromFile = argv[1];
            std::cout << "chromFile: " << chromFile << std::endl;
            std::string barCodeFile = argv[2];
            std::cout << "barCodeFile: " << barCodeFile << std::endl;
            std::string fragFile = argv[3];
            std::cout << "fragFile: " << fragFile << std::endl;
            std::string windEvalStr = argv[4];
            std::cout << "windEval: " << windEvalStr << std::endl;

            PeakBasedDist pkBsDist = PeakBasedDist::fromFlatFile(chromFile,barCodeFile);
            pkBsDist.addPeaksFromFragFile(fragFile,PeakBasedDist::strToUnsLong(windEvalStr));

            if (argc > 5) {
                std::string outBindFile = argv[5];
                std::cout << "outBindFile: " << outBindFile << std::endl;
                pkBsDist.write2BinaryFile(outBindFile);

            } else {
                pkBsDist.chrNormalize();
                PeakDistanceMatrix pkDistanceMatrix(pkBsDist.barCodeSet,pkBsDist);
                pkDistanceMatrix.writeMatrix();
            }
            return 0;}
        std::cerr << "Not enough arguments\n\n" << helpStrStr.str();
        return 1;}
    std::cerr << "No arguments\n\n" << helpStrStr.str();
    return 1;
}
