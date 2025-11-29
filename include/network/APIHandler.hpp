#ifndef APIHANDLER_HPP
#define APIHANDLER_HPP

#include "../dto/DataTransferObjects.hpp"
#include "../error/ErrorHandler.hpp"
#include "../core/BracketChecker.hpp"
#include <string>

class APIHandler {
private:
    ErrorHandler errorHandler;

public:
    APIHandler();
    ~APIHandler();  // Добавляем деструктор

    HttpResponse handleRequest(const HttpRequest& request);
    HttpResponse handleCheckBrackets(const std::string& requestBody);
    HttpResponse handleHealthCheck();
    HttpResponse handleNotFound();
    HttpResponse handleOptions();
};

#endif