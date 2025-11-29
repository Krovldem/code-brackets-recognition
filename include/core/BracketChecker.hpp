#ifndef BRACKETCHECKER_HPP
#define BRACKETCHECKER_HPP

#include "../error/ErrorHandler.hpp"
#include "Stack.hpp"
#include <string>
#include <vector>

class BracketChecker {
private:
    Stack stack;
    ErrorHandler& errorHandler;

    void processCharacter(char currentChar, int lineNumber, int position, const std::string& line);
    void validateRemainingBrackets();

public:
    explicit BracketChecker(ErrorHandler& errorHandler);

    bool checkFile(const std::string& filename);
    bool checkString(const std::string& code);
    void printResult() const;

    bool hasErrors() const { return errorHandler.hasErrors(); }
    std::vector<ErrorInfo> getErrors() const { return errorHandler.getErrors(); }
};

#endif