#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "PeakBasedDist.h"
#include "PeakDistanceMatrix.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::string argv1 = argv[1];
        if (argv1 == "-l") {
            std::string binFile = argv[2];
            PeakBasedDist pkBsDist = PeakBasedDist::fromBinFile(binFile);
            pkBsDist.chrNormalize();
            PeakDistanceMatrix pkDistanceMatrix(pkBsDist.barCodeSet,pkBsDist);
            std::cout << "bcvect1.size " << pkDistanceMatrix.barCodeVect1.size() << " bcvect2.size " << pkDistanceMatrix.barCodeVect2.size();
            std::cout << " flat matrix size " << pkDistanceMatrix.distanceFlatMatrix.size() << std::endl;
            auto itFlatMat = pkDistanceMatrix.distanceFlatMatrix.begin();
            for (std::string bC1:pkDistanceMatrix.barCodeVect1) {
                for (std::string bC2:pkDistanceMatrix.barCodeVect2) {
                    std::cout << bC1 << "\t" << bC2 << "\t" << *itFlatMat << std::endl;
                    ++itFlatMat;
                }
            }


        } else {
            if (argc > 5) {
                // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.

                // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.


                std::string chromFile = argv[1];
                std::cout << "chromFile: " << chromFile << std::endl;
                std::string barCodeFile = argv[2];
                std::cout << "barCodeFile: " << barCodeFile << std::endl;
                std::string fragFile = argv[3];
                std::cout << "fragFile: " << fragFile << std::endl;
                std::string windEvalStr = argv[4];
                std::cout << "windEval: " << windEvalStr << std::endl;
                std::string outBindFile = argv[5];
                std::cout << "outBindFile: " << outBindFile << std::endl;

                PeakBasedDist pkBsDist = PeakBasedDist::fromFlatFile(chromFile,barCodeFile);

                pkBsDist.addPeaksFromFragFile(fragFile,PeakBasedDist::strToUnsLong(windEvalStr));
                pkBsDist.chrNormalize();

                pkBsDist.write2BinaryFile(outBindFile);

                PeakDistanceMatrix pkDistanceMatrix(pkBsDist.barCodeSet,pkBsDist);
                std::cout << "bcvect1.size " << pkDistanceMatrix.barCodeVect1.size() << " bcvect2.size " << pkDistanceMatrix.barCodeVect2.size();
                std::cout << " flat matrix size " << pkDistanceMatrix.distanceFlatMatrix.size() << std::endl;
                auto itFlatMat = pkDistanceMatrix.distanceFlatMatrix.begin();
                for (std::string bC1:pkDistanceMatrix.barCodeVect1) {
                    for (std::string bC2:pkDistanceMatrix.barCodeVect2) {
                        std::cout << bC1 << "\t" << bC2 << "\t" << *itFlatMat << std::endl;
                        ++itFlatMat;
                    }
                }

                return 0;
            } else {
                std::cerr << "Missing arguments\n"
                        << "OpenChromDist\tchromFile\tbarCodeFile\tfragFile\twindEval\toutBindFile\n"
                << "chromFile contains\nCHROMOSOME=\nCHRLENGTH=\nBPSTEP=\nWINDSIZE=\n"
                << "barCodeFile contains a list of barcode separated by newline\n"
                        << "or \n"
                    <<  "OpenChromDist\t-l\tbinFile\n"
                    << std::endl;
                return 1;
            }
        }
    } else {
        std::cerr << "Missing arguments\n"
        << "For creating the distribution:\n"
                    << " OpenChromDist chromFile barCodeFile fragFile windEval outBindFile\n"
        << "  chromFile contains\n   CHROMOSOME=\n   CHRLENGTH=\n   BPSTEP=\n   WINDSIZE=\n"
                << "  barCodeFile contains a list of barcode separated by newline\n\n"
                    << "For downloading a distribution\n"
                <<  " OpenChromDist -l binFile\n"
                << std::endl;
        return 1;
    }
}