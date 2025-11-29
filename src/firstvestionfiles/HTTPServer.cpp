#include "firstvertionfiles/HTTPServer.hpp"
#include "firstvertionfiles/APIHandler.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

HTTPServer::HTTPServer(int port) : port(port), running(false) {}

HTTPServer::~HTTPServer() {
    stop();
}

void HTTPServer::start() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }
#endif

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }

    int opt = 1;
#ifdef _WIN32
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return;
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return;
    }

    std::cout << "Bracket Checker API Server running on http://localhost:" << port << std::endl;
    running = true;

    while (running) {
        sockaddr_in clientAddr;
#ifdef _WIN32
        int clientAddrLen = sizeof(clientAddr);
#else
        socklen_t clientAddrLen = sizeof(clientAddr);
#endif

        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            if (running) std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::thread clientThread(&HTTPServer::handleClient, this, clientSocket);
        clientThread.detach();
    }

#ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup();
#else
    close(serverSocket);
#endif
}

void HTTPServer::handleClient(int clientSocket) {
    char buffer[16384];
    memset(buffer, 0, sizeof(buffer));

#ifdef _WIN32
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
#else
    int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
#endif

    if (bytesRead > 0) {
        std::string request(buffer);
        std::string method, endpoint;
        std::string body = parseRequest(request, method, endpoint);

        std::string response = APIHandler::handleRequest(endpoint, method, body);
        std::string httpResponse = createResponse(response);

#ifdef _WIN32
        send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
        closesocket(clientSocket);
#else
        write(clientSocket, httpResponse.c_str(), httpResponse.length());
        close(clientSocket);
#endif
    }
}

std::string HTTPServer::parseRequest(const std::string& request, std::string& method, std::string& endpoint) {
    std::istringstream iss(request);
    iss >> method >> endpoint;

    std::string body;
    size_t bodyPos = request.find("\r\n\r\n");
    if (bodyPos != std::string::npos) {
        body = request.substr(bodyPos + 4);
    }

    return body;
}

std::string HTTPServer::createResponse(const std::string& content, const std::string& contentType) {
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: " << contentType << "\r\n"
             << "Access-Control-Allow-Origin: *\r\n"
             << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
             << "Access-Control-Allow-Headers: Content-Type\r\n"
             << "Content-Length: " << content.length() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n"
             << content;
    return response.str();
}

void HTTPServer::stop() {
    running = false;
}