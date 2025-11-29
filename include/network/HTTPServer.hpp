#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "APIHandler.hpp"
#include "RequestHandler.hpp"
#include <atomic>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

class HTTPServer {
private:
    int port;
    std::atomic<bool> running;
    APIHandler* apiHandler;  // Обычный указатель вместо unique_ptr

#ifdef _WIN32
    SOCKET serverSocket;
    WSADATA wsaData;
#else
    int serverSocket;
#endif

    void initializeSocket();
    void handleClient(int clientSocket);
    void cleanupSocket();

public:
    explicit HTTPServer(int port = 8080);
    ~HTTPServer();

    // Запрет копирования
    HTTPServer(const HTTPServer&) = delete;
    HTTPServer& operator=(const HTTPServer&) = delete;

    void start();
    void stop();
    bool isRunning() const { return running; }
};

#endif