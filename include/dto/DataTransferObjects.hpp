#ifndef DATATRANSFEROBJECTS_HPP
#define DATATRANSFEROBJECTS_HPP

#include <string>
#include <vector>

struct ErrorDTO {
    int line;
    int position;
    std::string message;
};

struct CheckResult {
    bool success;
    std::vector<ErrorDTO> errors;
};

struct HttpResponse {
    std::string content;
    std::string contentType;
    int statusCode;
};

struct HttpRequest {
    std::string method;
    std::string endpoint;
    std::string body;
    std::string contentType;
};

#endif