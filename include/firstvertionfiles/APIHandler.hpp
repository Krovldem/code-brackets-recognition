#ifndef APIHANDLER_HPP
#define APIHANDLER_HPP

#include <string>

class APIHandler {
public:
    static std::string handleRequest(const std::string& endpoint, const std::string& method, const std::string& body);
    static std::string handleCheckBrackets(const std::string& code);
};

#endif