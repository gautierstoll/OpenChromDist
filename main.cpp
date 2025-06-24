#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "PeakBasedDist.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Missing arguments" << std::endl;
        return 1;
    }
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.

    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.


    std::string chromFile = argv[1];
    std::cout << "chromFile: " << chromFile << std::endl;
    std::string barCodeFile = argv[2];
    std::cout << "barCodeFile: " << barCodeFile << std::endl;
    std::string fragFile = argv[3];
    std::cout << "fragile: " << fragFile << std::endl;

    PeakBasedDist pkBsDist = PeakBasedDist::fromFlatFile(chromFile,barCodeFile);

    pkBsDist.addPeaksFromFragFile(fragFile,10000); // value to test

    return 0;
}