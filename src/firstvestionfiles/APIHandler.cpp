#include "firstvertionfiles/APIHandler.hpp"
#include "firstvertionfiles/BracketChecker.hpp"
#include "firstvertionfiles/JSONSerializer.hpp"
#include <iostream>

std::string APIHandler::handleRequest(const std::string& endpoint, const std::string& method, const std::string& body) {
    std::cout << "API Request: " << method << " " << endpoint << std::endl;

    if (method == "POST" && endpoint == "/api/check") {
        return handleCheckBrackets(body);
    }
    else if (method == "GET" && endpoint == "/api/health") {
        return "{\"status\":\"ok\",\"service\":\"bracket-checker\"}";
    }

    return JSONSerializer::serializeError("Endpoint not found");
}

std::string APIHandler::handleCheckBrackets(const std::string& requestBody) {
    std::string code = JSONSerializer::extractCodeFromRequest(requestBody);

    if (code.empty()) {
        return JSONSerializer::serializeError("No code provided");
    }

    BracketChecker checker;
    bool success = checker.checkString(code);

    CheckResultDTO result;
    result.success = success;

    // Здесь нужно получить ошибки из checker
    // Для демонстрации добавляем тестовую ошибку
    if (!success) {
        ErrorDTO error;
        error.line = 1;
        error.position = 1;
        error.message = "Bracket mismatch detected";
        result.errors.push_back(error);
    }

    return JSONSerializer::serializeResult(result);
}