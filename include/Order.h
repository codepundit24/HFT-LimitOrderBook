#pragma once

#include <cstdint>

enum class OrderType{
    BUY,
    SELL
};

struct Order
{
    uint64_t orderId;
    OrderType side;
    double price;
    uint32_t quantity;
    uint64_t timestamp;

    //Intrusive Doubly Linked List pointers for O(1) queue management
    Order* prev = nullptr;
    Order* next = nullptr;

    Order(uint64_t id,OrderType s, double p, uint32_t q, uint64_t ts)
         : orderId(id),side(s),price(p),quantity(q),timestamp(ts) {}
};
