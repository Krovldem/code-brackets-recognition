#ifndef JSONSERIALIZER_HPP
#define JSONSERIALIZER_HPP

#include "../dto/DataTransferObjects.hpp"
#include "../error/ErrorTypes.hpp"
#include <string>
#include <vector>

class JSONSerializer {
public:
    static std::string serializeResult(const CheckResult& result);
    static std::string serializeError(const std::string& message);
    static std::string extractCodeFromRequest(const std::string& requestBody);
    static std::vector<ErrorDTO> convertToDTO(const std::vector<ErrorInfo>& errors);
};

#endif