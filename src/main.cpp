#include <iostream>
#include "OrderBook.h"
#include "OrderPool.h"

int main(){
    OrderPool pool(100000);
    OrderBook book;

    std::cout<<"--- HFT Limit Order Book Initialized ---"<<std::endl;

    //Sell order: 100 shares @ $150.00
    Order* sellOrder = new Order(1,OrderType::SELL,150.00, 100, 1001);
    book.addOrder(sellOrder);

    //Buy Order:50 shares @ $148.00 (Can't match as price is low)
    Order* lowBuyOrder = new Order(2, OrderType::BUY, 148.00, 50, 1002);
    book.addOrder(lowBuyOrder);

    book.printBook();

    //Buy order: 100 shares @150 (Exact match , so execution will trigger)
    Order* matchingBuyOrder = new Order(3, OrderType::BUY, 150,100,1003);
    std::cout << "\nAdding Matching Buy Order(150)..."<<std::endl;
    book.addOrder(matchingBuyOrder);

    book.printBook();

    //When order work done , return it to pool
    //When automatically delete happen in real system or orderbook 
    pool.deallocate(sellOrder);
    pool.deallocate(lowBuyOrder);
    pool.deallocate(matchingBuyOrder);

    return 0;
}