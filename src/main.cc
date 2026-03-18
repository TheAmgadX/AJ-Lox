#include <iostream>
#include "aj_lox.h"

int main(int argc, char* argv[]) {
    if(argc > 2) {
        std::cout << "Usage: AJ_Lox: [script]\n";
    } else if(argc == 2) {
        AJ_Lox::runFile(argv[1]);
    } else {
        AJ_Lox::runPrompt();
    }
}
