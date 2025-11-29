#ifndef BRACKETCHECKER_HPP
#define BRACKETCHECKER_HPP

#include "Stack.hpp"
#include "ErrorLogger.hpp"
#include <string>
#include <vector>

class BracketChecker {
private:
    Stack stack;
    ErrorLogger logger;

    void processCharacter(char currentChar, int lineNumber, int position, const std::string& line);
    void validateRemainingBrackets();

public:
    BracketChecker(const std::string& errorFile = "errors.txt");

    bool checkFile(const std::string& filename);
    bool checkString(const std::string& code);  // Добавить этот метод
    void printResult() const;

    std::vector<ErrorInfo> getErrors() const;
};

#endif