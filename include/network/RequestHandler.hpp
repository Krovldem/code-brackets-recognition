#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "../dto/DataTransferObjects.hpp"
#include <string>

class RequestHandler {
public:
    static HttpRequest parseRequest(const std::string& rawRequest);
    static std::string createHttpResponse(const HttpResponse& response);
};

#endif