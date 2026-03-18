#pragma  once
#include <iostream>

class IErrorReporter {
public:
    virtual void report(const int line, const std::string& where, const std::string& message) = 0;
};

class ConsoleErrorReporter : public IErrorReporter {
public:
    void report(const int line, const std::string& where, const std::string& message) override {
        std::cerr << "\033[31m[line " << line << "] " << where << ": " << message << "\033[0m\n";
    }
};
