#pragma once
#include "error_reporter.h"
#include <string>

class AJ_Lox {
private:
    static bool m_HasError;
    static IErrorReporter* p_ErrorReporter;

    static void run(std::string& source);

    static void report(const int line, const std::string& where, const std::string& message);

public:
    static void error(const int line, const std::string& message);

    static void runFile(const char* file);

    static void runPrompt();

    static void setErrorReporter(IErrorReporter* reporter) {
        p_ErrorReporter = reporter;
    }

};
