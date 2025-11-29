#include "../../include/network/APIHandler.hpp"
#include "../../include/serialization/JSONSerializer.hpp"
#include <iostream>

APIHandler::APIHandler() : errorHandler("api_errors.txt") {}

APIHandler::~APIHandler() {
    // Деструктор для очистки ресурсов если нужно
}

HttpResponse APIHandler::handleRequest(const HttpRequest& request) {
    std::cout << "API Request: " << request.method << " " << request.endpoint << std::endl;

    if (request.method == "POST" && request.endpoint == "/api/check") {
        return handleCheckBrackets(request.body);
    }
    else if (request.method == "GET" && request.endpoint == "/api/health") {
        return handleHealthCheck();
    }
    else if (request.method == "OPTIONS") {
        return handleOptions();
    }

    return handleNotFound();
}

HttpResponse APIHandler::handleCheckBrackets(const std::string& requestBody) {
    std::string code = JSONSerializer::extractCodeFromRequest(requestBody);

    if (code.empty()) {
        return HttpResponse{
                JSONSerializer::serializeError("No code provided"),
                "application/json",
                400
        };
    }

    errorHandler.clearErrors();
    BracketChecker checker(errorHandler);
    bool success = checker.checkString(code);

    CheckResult result;
    result.success = success;
    result.errors = JSONSerializer::convertToDTO(errorHandler.getErrors());

    return HttpResponse{
            JSONSerializer::serializeResult(result),
            "application/json",
            200
    };
}

HttpResponse APIHandler::handleHealthCheck() {
    return HttpResponse{
            "{\"status\":\"ok\",\"service\":\"bracket-checker\"}",
            "application/json",
            200
    };
}

HttpResponse APIHandler::handleNotFound() {
    return HttpResponse{
            JSONSerializer::serializeError("Endpoint not found"),
            "application/json",
            404
    };
}

HttpResponse APIHandler::handleOptions() {
    return HttpResponse{
            "",
            "text/plain",
            200
    };
}