#include "Server.h"
#include <string>

Server::Server(int p, OrderBook& book, OrderPool& pool) 
    : port(p), serverSocket(INVALID_SOCKET), orderBook(book), orderPool(pool), isRunning(false) {}

Server::~Server() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
    WSACleanup();
}

bool Server::start() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    isRunning = true;
    std::cout << "TCP Server started successfully on port " << port << "...\n";
    return true;
}

void Server::listenForConnections() {
    while (isRunning) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            break;
        }

        std::cout << "Client connected successfully!\n";

        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string message(buffer);
            std::cout << "Received data from client: " << buffer << std::endl;
            
            //Default values
            OrderType oType =OrderType::LIMIT;
            Side oSide = Side::BUY;
            double price = 0;
            double triggerPrice = 0;
            uint32_t qty = 0;
            uint64_t orderId = rand() % 100000;

            if (message.find("STOP_LOSS") != std::string::npos){
                oType = OrderType::STOP_LOSS;
            }
            
            if (message.find("SELL") != std::string::npos){
                oSide = Side::SELL;
            }

            size_t atPos = message.find('@');
            size_t trigPos = message.find("Trigger:");
            size_t qtyPos = message.find("Qty");

            if (atPos != std::string::npos){
                price = std::stod(message.substr(atPos + 1));
            }
            if (trigPos != std::string::npos){
                triggerPrice = std::stod(message.substr(trigPos + 8));
            }
            if ( qtyPos != std::string::npos){
                qty = static_cast<uint32_t>(std::stoi(message.substr(qtyPos + 4)));
            }

            Order* newOrder = orderPool.allocate(orderId,oSide,oType,price,triggerPrice,qty,time(nullptr));
            if (newOrder){
                orderBook.addOrder(newOrder);
                std::cout<< "Order dynamically allocated and added to book successfully!\n";
            }
            
            const char* response = "Order processed by C++ Engine\n";
            send(clientSocket, response, (int)strlen(response), 0);
        }

        closesocket(clientSocket);
    }
}