#include <iostream>
#include "aj_lox.h"
#include "error_reporter.h"

int main(int argc, char* argv[]) {
    ConsoleErrorReporter* console_reporter = new ConsoleErrorReporter();
    AJ_Lox::setErrorReporter(console_reporter);

    if(argc > 2) {
        std::cout << "Usage: AJ_Lox: [script]\n";
    } else if(argc == 2) {
        AJ_Lox::runFile(argv[1]);
    } else {
        AJ_Lox::runPrompt();
    }
}
