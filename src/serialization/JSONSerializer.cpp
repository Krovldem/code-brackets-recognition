#include "../../include/serialization/JSONSerializer.hpp"
#include <sstream>

std::string JSONSerializer::serializeResult(const CheckResult& result) {
    std::stringstream json;
    json << "{\"success\":" << (result.success ? "true" : "false") << ",\"errors\":[";

    for (size_t i = 0; i < result.errors.size(); ++i) {
        const auto& error = result.errors[i];
        json << "{\"line\":" << error.line
             << ",\"position\":" << error.position
             << ",\"message\":\"" << error.message << "\"}";
        if (i < result.errors.size() - 1) {
            json << ",";
        }
    }

    json << "]}";
    return json.str();
}

std::string JSONSerializer::serializeError(const std::string& message) {
    return "{\"error\":\"" + message + "\"}";
}

std::string JSONSerializer::extractCodeFromRequest(const std::string& requestBody) {
    size_t codeStart = requestBody.find("\"code\":\"");
    if (codeStart == std::string::npos) return "";

    codeStart += 8;
    size_t codeEnd = requestBody.find("\"", codeStart);
    if (codeEnd == std::string::npos) return "";

    std::string code = requestBody.substr(codeStart, codeEnd - codeStart);

    // Replace escaped characters
    size_t pos = 0;
    while ((pos = code.find("\\n", pos)) != std::string::npos) {
        code.replace(pos, 2, "\n");
        pos += 1;
    }

    // Replace other escaped characters if needed
    pos = 0;
    while ((pos = code.find("\\t", pos)) != std::string::npos) {
        code.replace(pos, 2, "\t");
        pos += 1;
    }

    return code;
}

std::vector<ErrorDTO> JSONSerializer::convertToDTO(const std::vector<ErrorInfo>& errors) {
    std::vector<ErrorDTO> dtos;
    for (const auto& error : errors) {
        dtos.push_back({error.lineNumber, error.position, error.message});
    }
    return dtos;
}