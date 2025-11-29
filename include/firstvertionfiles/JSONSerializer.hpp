#ifndef JSONSERIALIZER_HPP
#define JSONSERIALIZER_HPP

#include <string>
#include <vector>

struct ErrorDTO {
    int line;
    int position;
    std::string message;
};

struct CheckResultDTO {
    bool success;
    std::vector<ErrorDTO> errors;
};

class JSONSerializer {
public:
    static std::string serializeResult(const CheckResultDTO& result);
    static std::string serializeError(const std::string& message);
    static std::string extractCodeFromRequest(const std::string& requestBody);
};

#endif