#ifndef ERRORLOGGER_HPP
#define ERRORLOGGER_HPP

#include <string>
#include <fstream>
#include <vector>

struct ErrorInfo {
    int lineNumber;
    int position;
    std::string message;
    std::string context;
};

class ErrorLogger {
private:
    std::string outputFile;
    std::vector<ErrorInfo> errors;

public:
    ErrorLogger(const std::string& filename);

    void logError(int lineNumber, int position, const std::string& message, const std::string& context = "");  // Исправить сигнатуру
    void saveToFile();
    void printErrors() const;
    bool hasErrors() const;
    int getErrorCount() const;
    std::vector<ErrorInfo> getErrors() const;  // Добавить этот метод
};

#endif