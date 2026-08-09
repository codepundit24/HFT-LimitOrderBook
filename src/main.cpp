#include <iostream>
#include "OrderBook.h"
#include "OrderPool.h"

int main(){
    OrderPool pool(100000);
    OrderBook book;

    std::cout<<"--- Testing Intrusive Linked List & Queues ---"<<std::endl;

    //3 order placed at same price 
    Order* buy1= pool.allocate(101, OrderType::BUY, 148, 50, 1001);
    Order* buy2= pool.allocate(102, OrderType::BUY, 148, 100, 1002);
    Order* buy3= pool.allocate(103, OrderType::BUY, 148, 75, 1003);

    book.addOrder(buy1);
    book.addOrder(buy2);
    book.addOrder(buy3);

    std::cout<<"\nAfter adding 3 Bids at $148"<<std::endl;
    book.printBook();

    std::cout<<"\nCancelling Order ID 102"<<std::endl;
    book.cancelOrder(102);

    book.printBook();

    Order* sell1= pool.allocate(201, OrderType::SELL, 148, 80, 1004);
    std::cout<<"\nIncoming Sell Order for 80 shares @ $148 "<<std::endl;
    book.addOrder(sell1);

    book.printBook();


    return 0;
}