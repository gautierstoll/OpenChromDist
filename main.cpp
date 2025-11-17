#include <fstream>
#include <iostream>
#include <sstream>

#include "CellDataMeanVar.h"
#include "version.h"

#include "PeakBasedDist.h"
#include "PeakDistanceMatrix.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main(int argc, char* argv[]) {
    std::ostringstream helpStrStr;
    helpStrStr << "Project version: " << PROJECT_VERSION << "\n\n"
    << "For creating distribution:\n"
    << "  OpenChromDist chromFile barCodeFile fragFile windEval outBindFile\n\n"
    << "    chromFile contains\n      CHROMOSOME=\n      CHRLENGTH=\n      BPSTEP=\n      WINDSIZE=\n"
    << "    barCodeFile contains a list of barcode separated by newline\n"
    << "    with no output binary file, distance matrix is directly computed\n\n"
    << "For computing distance matrix from binary file of distribution: \n"
    <<  "  OpenChromDist -l binFile (optional)outMatrixFile\n\n"
    << "For processing fragment file: \n"
    << " OpenChromDist -p barCodeFile fragFile\n\n"
    << "For help:\n"
    << "  OpenChromDist -h\n"
    << std::endl;

    if (argc > 1) {
        const std::string argv1 = argv[1];
        if (argv1 == "-h") {
            std::cout << helpStrStr.str();
            return 0;}
        if (argv1 == "-l") {
            if (argc > 2) {
                const std::string binFile = argv[2];
                std::optional<std::string> fileName = std::nullopt;
                if (argc > 3) {fileName = argv[3]; }
                PeakBasedDist pkBsDist = PeakBasedDist::fromBinFile(binFile);
                pkBsDist.chrNormalize();
                PeakDistanceMatrix pkDistanceMatrix(pkBsDist.barCodeSet,pkBsDist);
                std::cout << "start writing distance matrix";
                pkDistanceMatrix.writeMatrix(fileName);
                return 0;}
            std::cerr << "No binary file\n\n" << helpStrStr.str();
            return 1;}
        if (argv1 == "-p") {
            if (argc > 3) {
                const std::string barCodeFile = argv[2];
                const std::string frFile = argv[3];
                CellDataMeanVar CDMeanVar = CellDataMeanVar::fromFlatFile(barCodeFile);
                CDMeanVar.readFragFile(frFile);
                CDMeanVar.toCout();
            }
            std::cerr << "No barCodeFile and/or fragFile\n\n" << helpStrStr.str();
        }
        if (argc > 4)
        {
            const std::string chromFile = argv[1];
            std::cout << "chromFile: " << chromFile << std::endl;
            const std::string barCodeFile = argv[2];
            std::cout << "barCodeFile: " << barCodeFile << std::endl;
            const std::string fragFile = argv[3];
            std::cout << "fragFile: " << fragFile << std::endl;
            const std::string windEvalStr = argv[4];
            std::cout << "windEval: " << windEvalStr << std::endl;

            PeakBasedDist pkBsDist = PeakBasedDist::fromFlatFile(chromFile,barCodeFile);
            pkBsDist.addPeaksFromFragFile(fragFile,PeakBasedDist::strToUnsLong(windEvalStr));

            if (argc > 5) {
                const std::string outBindFile = argv[5];
                std::cout << "outBindFile: " << outBindFile << std::endl;
                pkBsDist.write2BinaryFile(outBindFile);

            } else {
                pkBsDist.chrNormalize();
                PeakDistanceMatrix pkDistanceMatrix(pkBsDist.barCodeSet,pkBsDist);
                pkDistanceMatrix.writeMatrix();
            }
            return 0;}
        std::cerr << "Error, not enough arguments\n\n" << helpStrStr.str();
        return 1;}
    std::cerr << "Error, no arguments\n\n" << helpStrStr.str();
    return 1;
}
