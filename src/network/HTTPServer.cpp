#include "../../include/network/HTTPServer.hpp"
#include <iostream>
#include <cstring>
#include <thread>
#include <stdexcept>

HTTPServer::HTTPServer(int port) : port(port), running(false) {
#ifdef _WIN32
    serverSocket = INVALID_SOCKET;
#else
    serverSocket = -1;
#endif
    apiHandler = new APIHandler();
}

HTTPServer::~HTTPServer() {
    stop();
    if (apiHandler) {
        delete apiHandler;
        apiHandler = nullptr;
    }
}

void HTTPServer::initializeSocket() {
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        throw std::runtime_error("Socket creation failed");
    }
#else
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Socket creation failed");
    }
#endif

    int opt = 1;
#ifdef _WIN32
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Setsockopt failed");
    }
#else
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Setsockopt failed");
    }
#endif

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(serverSocket, 10) < 0) {
        throw std::runtime_error("Listen failed");
    }
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
        HttpRequest httpRequest = RequestHandler::parseRequest(request);

        HttpResponse httpResponse = apiHandler->handleRequest(httpRequest);
        std::string responseStr = RequestHandler::createHttpResponse(httpResponse);

#ifdef _WIN32
        send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
        closesocket(clientSocket);
#else
        write(clientSocket, responseStr.c_str(), responseStr.length());
        close(clientSocket);
#endif
    } else {
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
    }
}

void HTTPServer::cleanupSocket() {
#ifdef _WIN32
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
    WSACleanup();
#else
    if (serverSocket >= 0) {
        close(serverSocket);
        serverSocket = -1;
    }
#endif
}

void HTTPServer::start() {
    try {
        initializeSocket();

        std::cout << "✅ Bracket Checker API Server running on http://localhost:" << port << std::endl;
        std::cout << "✅ Endpoints:" << std::endl;
        std::cout << "   - POST /api/check - Check brackets in code" << std::endl;
        std::cout << "   - GET  /api/health - Health check" << std::endl;
        std::cout << "Waiting for requests..." << std::endl;

        running = true;

        while (running) {
            sockaddr_in clientAddr;
#ifdef _WIN32
            int clientAddrLen = sizeof(clientAddr);
            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
            if (clientSocket == INVALID_SOCKET) {
#else
                socklen_t clientAddrLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
            if (clientSocket < 0) {
#endif
                if (running) {
                    std::cerr << "Accept failed" << std::endl;
                }
                continue;
            }

            std::thread clientThread(&HTTPServer::handleClient, this, clientSocket);
            clientThread.detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        cleanupSocket();
        throw;
    }

    cleanupSocket();
}

void HTTPServer::stop() {
    running = false;
#ifdef _WIN32
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
#else
    if (serverSocket >= 0) {
        close(serverSocket);
    }
#endif
}