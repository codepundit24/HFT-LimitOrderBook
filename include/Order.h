#pragma once
#ifndef ORDER_H
#define ORDER_H

#include <cstdint>

enum class OrderType{
   LIMIT,
   MARKET,
   STOP_LOSS
};

enum class Side{
    BUY,
    SELL
};

struct Order
{
    uint64_t orderId;
    Side side;
    OrderType type;
    double price;
    double trigger_price;
    uint32_t quantity;
    uint32_t filled_quantity;
    uint64_t timestamp;

    //Intrusive Doubly Linked List pointers for O(1) queue management
    Order* prev = nullptr;
    Order* next = nullptr;

    Order(uint64_t id,Side s,OrderType t, double p,double trig_p, uint32_t qty, uint64_t ts)
         : orderId(id),side(s),type(t), price(p),trigger_price(trig_p), quantity(qty),
         filled_quantity(0), timestamp(ts) {}
};
#endif