#pragma once

#include <map>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "Order.h"


//New struct for all order at same price
struct OrderList
{
    /* data */
    Order* head = nullptr;
    Order* tail = nullptr;

    //Time priority , add at end of line
    void append(Order* order){
        if(!head){
            head = tail = order;
            order->prev = nullptr;
            order->next = nullptr;
        }else {
            tail->next = order;
            order->prev = tail;
            order->next = nullptr;
            tail = order;
        }
    }

    //In O(1) time , pick out any order 
    void remove(Order* order){
        if (order->prev) order->prev->next = order->next;
        if (order->next) order->next->prev = order->prev;

        if (head == order) head = order->next;
        if (tail == order) tail = order->prev;
    }
    bool isEmpty() const { return head == nullptr; }
};

class OrderBook{
private:
    //Bids (Buy side): Highest price first
    std::map<double,OrderList, std::greater<double>> bids;

    //Asks (Sell side): Lowest price fist
    std::map<double,OrderList, std::less<double>> asks;

    //Fast O(1) lookup for cancels/searches
    std::unordered_map<uint64_t,Order*> orderLookup;

    std::vector<Order*> stopLossOrders;

public:
    void addOrder(Order* order);
    void checkStopLossTriggers(double lastTraderPrice);
    void cancelOrder(uint64_t orderId); 
    void matchOrders();
    void printBook() const;
};