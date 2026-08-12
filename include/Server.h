#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include<iostream>
#include "OrderBook.h"
#include "OrderPool.h"

#pragma comment(lib, "Ws2_32.lib")

class Server {
private:
    int port;
    SOCKET serverSocket;
    OrderBook& orderBook;
    OrderPool& orderPool;
    bool isRunning;

public:
    Server(int p, OrderBook& book,OrderPool& pool);
    ~Server();
    bool start();
    void listenForConnections();
};

#endif
