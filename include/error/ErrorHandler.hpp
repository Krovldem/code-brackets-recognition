#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include "ErrorTypes.hpp"
#include <vector>
#include <string>

class ErrorHandler {
private:
    std::string outputFile;
    std::vector<ErrorInfo> errors;

public:
    explicit ErrorHandler(const std::string& filename = "errors.txt");

    void logError(int lineNumber, int position, const std::string& message,
                  const std::string& context = "", ErrorSeverity severity = ErrorSeverity::ERROR);
    void clearErrors();
    void saveToFile() const;
    void printErrors() const;

    bool hasErrors() const;
    int getErrorCount() const;
    std::vector<ErrorInfo> getErrors() const;

    static ErrorInfo createSyntaxError(int line, int pos, const std::string& details);
    static ErrorInfo createBracketMismatchError(int line, int pos, char expected, char found);
};

#endif