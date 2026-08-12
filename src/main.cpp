#include <iostream>
#include "OrderBook.h"
#include "OrderPool.h"
#include "Server.h"

int main(){

    std::cout<<"--- INITIALIZING HFT LIMIT ORDER BOOK & TCP SERVER---\n";

    OrderPool orderPool(10000);

    OrderBook orderBook;

    int port = 8080;
    Server server(port,orderBook,orderPool);

    if (!server.start()){
        std::cerr<< "Failed to start TCP server on port "<<port<<std::endl;
        return 1;
    }

    server.listenForConnections();

    return 0;
}