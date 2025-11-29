#ifndef ERRORTYPES_HPP
#define ERRORTYPES_HPP

#include <string>

struct ErrorInfo {
    int lineNumber;
    int position;
    std::string message;
    std::string context;
};

enum class ErrorSeverity {
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

#endif