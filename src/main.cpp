#include "../include/network/HTTPServer.hpp"
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <memory>

HTTPServer* server = nullptr;

void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    if (server) {
        server->stop();
        delete server;
        server = nullptr;
    }
}

int main() {
    std::cout << "=== Starting C++ Bracket Checker HTTP Server ===" << std::endl;

    // Установка обработчиков сигналов
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    try {
        // Используем обычный new вместо make_unique
        server = new HTTPServer(8080);
        server->start();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        if (server) {
            delete server;
            server = nullptr;
        }
        return 1;
    }

    if (server) {
        delete server;
        server = nullptr;
    }

    std::cout << "Server stopped gracefully." << std::endl;
    return 0;
}