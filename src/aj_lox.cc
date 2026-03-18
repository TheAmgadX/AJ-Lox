#include "aj_lox.h"
#include "error_reporter.h"
#include "scanner.h"
#include "token.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool AJ_Lox::m_HasError = false;
IErrorReporter* AJ_Lox::p_ErrorReporter = nullptr;

// TODO: search for a way to optimize this function.
void AJ_Lox::runFile(const char *filePath) {
    m_HasError = false;

    std::cout << "\033[31m Running file: " << filePath << "\033[0m\n";
    std::ifstream file(filePath);

    if (!file.is_open()) {
        error(0, "Could not open file: " + std::string(filePath));
        return;
    }

    std::string line;
    std::string source;

    while (std::getline(file, line)) {
        source += line + "\n";
    }

    file.close();

    run(source);

    if(m_HasError) {
        error(0, "Compilation failed.");
        return;
    }
}

void AJ_Lox::runPrompt() {
    std::string file_path;

    while(true) {
        std::cout << "\033[32m>>> Enter File Path:\n" << "\033[32m>>> ";
        std::getline(std::cin, file_path);
        if(file_path == "") break;
        runFile(file_path.c_str());
    }
}

void AJ_Lox::run(std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens;
    scanner.scanTokens(tokens);

    // for now just print the tokens, this will be replaced with the parser
    for (const Token& token : tokens) {
        token.printToken();
    }
}

void AJ_Lox::error(const int line, const std::string& message) {
    if(p_ErrorReporter)
        p_ErrorReporter->report(line, "", message);

    m_HasError = false;
}
