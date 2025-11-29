#include "../../include/error/ErrorHandler.hpp"
#include <iostream>
#include <fstream>

ErrorHandler::ErrorHandler(const std::string& filename) : outputFile(filename) {}

void ErrorHandler::logError(int lineNumber, int position, const std::string& message,
                            const std::string& context, ErrorSeverity severity) {
    errors.push_back({lineNumber, position, message, context});
}

void ErrorHandler::clearErrors() {
    errors.clear();
}

void ErrorHandler::saveToFile() const {
    std::ofstream file(outputFile);
    if (!file.is_open()) return;

    for (const auto& error : errors) {
        file << "Line " << error.lineNumber << ", Position " << error.position << ": ";
        file << error.message << std::endl;
        if (!error.context.empty()) {
            file << "Context: " << error.context << std::endl;
        }
    }
    file.close();
}

void ErrorHandler::printErrors() const {
    for (const auto& error : errors) {
        std::cout << "Line " << error.lineNumber << ", Position " << error.position << ": ";
        std::cout << error.message << std::endl;
        if (!error.context.empty()) {
            std::cout << "Context: " << error.context << std::endl;
        }
    }
}

bool ErrorHandler::hasErrors() const {
    return !errors.empty();
}

int ErrorHandler::getErrorCount() const {
    return errors.size();
}

std::vector<ErrorInfo> ErrorHandler::getErrors() const {
    return errors;
}

ErrorInfo ErrorHandler::createSyntaxError(int line, int pos, const std::string& details) {
    return {line, pos, "Syntax error: " + details, ""};
}

ErrorInfo ErrorHandler::createBracketMismatchError(int line, int pos, char expected, char found) {
    return {line, pos,
            "Bracket mismatch: expected '" + std::string(1, expected) +
            "', found '" + std::string(1, found) + "'", ""};
}