#pragma once

#include <map>
#include <unordered_map>
#include <iostream>
#include "Order.h"

class OrderBook{
private:
    //Bids (Buy side): Highest price first
    std::map<double,Order*, std::greater<double>> bids;

    //Asks (Sell side): Lowest price fist
    std::map<double,Order*, std::less<double>> asks;

    //Fast O(1) lookup for cancels/searches
    std::unordered_map<uint64_t,Order*> orderLookup;

public:
    void addOrder(Order* order);
    void matchOrders();
    void printBook() const;
};