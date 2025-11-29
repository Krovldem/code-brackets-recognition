#include "../../include/network/RequestHandler.hpp"
#include <sstream>
#include <iostream>

HttpRequest RequestHandler::parseRequest(const std::string& rawRequest) {
    HttpRequest request;
    std::istringstream iss(rawRequest);

    iss >> request.method >> request.endpoint;

    std::string line;
    while (std::getline(iss, line) && line != "\r") {
        if (line.find("Content-Type:") != std::string::npos) {
            request.contentType = line.substr(14);
            if (!request.contentType.empty() && request.contentType.back() == '\r') {
                request.contentType.pop_back();
            }
        }
    }

    // Parse body
    size_t bodyPos = rawRequest.find("\r\n\r\n");
    if (bodyPos != std::string::npos) {
        request.body = rawRequest.substr(bodyPos + 4);
    }

    return request;
}

std::string RequestHandler::createHttpResponse(const HttpResponse& response) {
    std::stringstream httpResponse;

    httpResponse << "HTTP/1.1 " << response.statusCode << " ";

    // Add status message based on code
    switch (response.statusCode) {
        case 200: httpResponse << "OK"; break;
        case 400: httpResponse << "Bad Request"; break;
        case 404: httpResponse << "Not Found"; break;
        case 500: httpResponse << "Internal Server Error"; break;
        default: httpResponse << "Unknown"; break;
    }

    httpResponse << "\r\n"
                 << "Content-Type: " << response.contentType << "\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                 << "Access-Control-Allow-Headers: Content-Type\r\n"
                 << "Content-Length: " << response.content.length() << "\r\n"
                 << "Connection: close\r\n"
                 << "\r\n"
                 << response.content;

    return httpResponse.str();
}