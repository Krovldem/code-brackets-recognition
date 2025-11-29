#include "firstvertionfiles/ErrorLogger.hpp"
#include <iostream>

ErrorLogger::ErrorLogger(const std::string& filename) : outputFile(filename) {}

void ErrorLogger::logError(int lineNumber, int position, const std::string& message, const std::string& context) {
    errors.push_back({lineNumber, position, message, context});
}

void ErrorLogger::saveToFile() {
    std::ofstream file(outputFile);
    if (!file.is_open()) return;

    for (const auto& error : errors) {
        file << "Line " << error.lineNumber << ", Position " << error.position << ": ";
        file << error.message << std::endl;
        if (!error.context.empty()) {
            file << error.context << std::endl;
        }
    }

    file.close();
}

void ErrorLogger::printErrors() const {
    for (const auto& error : errors) {
        std::cout << "Line " << error.lineNumber << ", Position " << error.position << ": ";
        std::cout << error.message << std::endl;
        if (!error.context.empty()) {
            std::cout << error.context << std::endl;
        }
    }
}

bool ErrorLogger::hasErrors() const {
    return !errors.empty();
}

int ErrorLogger::getErrorCount() const {
    return errors.size();
}

std::vector<ErrorInfo> ErrorLogger::getErrors() const {
    return errors;
}