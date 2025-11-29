#include "../../include/core/BracketChecker.hpp"
#include "../../include/error/ErrorHandler.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

BracketChecker::BracketChecker(ErrorHandler& errorHandler) : errorHandler(errorHandler) {}

bool BracketChecker::checkFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        errorHandler.logError(0, 0, "Cannot open file");
        return false;
    }

    std::string line;
    int lineNumber = 0;
    bool inSpecialComment = false;
    int commentStartLine = 0;
    int commentStartPos = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        int i = 0;

        while (i < line.length()) {
            char currentChar = line[i];

            if (!inSpecialComment) {
                if (currentChar == '(' && i + 1 < line.length() && line[i + 1] == '*') {
                    inSpecialComment = true;
                    commentStartLine = lineNumber;
                    commentStartPos = i + 1;
                    i = i + 2;
                    continue;
                }
            } else {
                if (currentChar == '*' && i + 1 < line.length() && line[i + 1] == ')') {
                    inSpecialComment = false;
                    i = i + 2;
                    continue;
                }
            }

            if (inSpecialComment) {
                i = i + 1;
                continue;
            }

            processCharacter(currentChar, lineNumber, i + 1, line);
            i = i + 1;
        }
    }

    file.close();

    if (inSpecialComment) {
        errorHandler.logError(commentStartLine, commentStartPos, "Unclosed special comment");
    }

    validateRemainingBrackets();

    return !errorHandler.hasErrors();
}

bool BracketChecker::checkString(const std::string& code) {
    std::stringstream ss(code);
    std::string line;
    int lineNumber = 0;
    bool inSpecialComment = false;
    int commentStartLine = 0;
    int commentStartPos = 0;

    while (std::getline(ss, line)) {
        lineNumber++;
        int i = 0;

        while (i < line.length()) {
            char currentChar = line[i];

            if (!inSpecialComment) {
                if (currentChar == '(' && i + 1 < line.length() && line[i + 1] == '*') {
                    inSpecialComment = true;
                    commentStartLine = lineNumber;
                    commentStartPos = i + 1;
                    i = i + 2;
                    continue;
                }
            } else {
                if (currentChar == '*' && i + 1 < line.length() && line[i + 1] == ')') {
                    inSpecialComment = false;
                    i = i + 2;
                    continue;
                }
            }

            if (inSpecialComment) {
                i = i + 1;
                continue;
            }

            processCharacter(currentChar, lineNumber, i + 1, line);
            i = i + 1;
        }
    }

    if (inSpecialComment) {
        errorHandler.logError(commentStartLine, commentStartPos, "Unclosed special comment");
    }

    validateRemainingBrackets();

    return !errorHandler.hasErrors();
}

void BracketChecker::processCharacter(char currentChar, int lineNumber, int position, const std::string& line) {
    if (currentChar == '(' || currentChar == '[' || currentChar == '{') {
        stack.push(currentChar, lineNumber, position);
    }
    else if (currentChar == ')' || currentChar == ']' || currentChar == '}') {
        if (stack.isEmpty()) {
            errorHandler.logError(lineNumber, position, "Unexpected closing bracket");
        } else {
            StackNode* topNode = stack.peek();
            char topBracket = topNode->bracket;

            if ((topBracket == '(' && currentChar == ')') ||
                (topBracket == '[' && currentChar == ']') ||
                (topBracket == '{' && currentChar == '}')) {
                stack.pop();
            } else {
                errorHandler.logError(lineNumber, position, "Mismatched bracket");
            }
        }
    }
}

void BracketChecker::validateRemainingBrackets() {
    while (!stack.isEmpty()) {
        StackNode* topNode = stack.peek();
        errorHandler.logError(topNode->lineNumber, topNode->position, "Unclosed bracket");
        stack.pop();
    }
}

void BracketChecker::printResult() const {
    if (!errorHandler.hasErrors()) {
        std::cout << "SUCCESS: All brackets are properly balanced!" << std::endl;
    } else {
        std::cout << "FAILURE: Found bracket errors" << std::endl;
        errorHandler.printErrors();
    }
}