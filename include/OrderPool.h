#pragma once
#include <vector>
#include <stack>
#include "Order.h"

class OrderPool {
private:
    std::vector<Order> pool;
    std::stack<Order*> freeList;

public:
    //Constructor: Pool size allocation
    OrderPool(size_t poolSize = 100000){
        //Memory allocation with dummy data
        pool.reserve(poolSize);
        for (size_t i=0;i< poolSize;i++){
            pool.emplace_back(0,Side::BUY,OrderType::LIMIT,0.0, 0.0, 0,0);
            freeList.push(&pool[i]);
        }
    } 

    //O(1) Allocation: Picking empty slot instead of new
    Order* allocate(uint64_t id, Side side,OrderType type,double price,double trigger_price, uint32_t qty, uint64_t ts){
        if(freeList.empty()){
            return nullptr;
        }

        Order* order = freeList.top();
        freeList.pop();

        //Overwrite empty slot with new data
        order->orderId = id;
        order->side = side;
        order->type = type;
        order->price = price;
        order->trigger_price = trigger_price;
        order->quantity=qty;
        order->filled_quantity = 0;
        order->timestamp = ts;
        order->next = nullptr;
        order->prev = nullptr;

        return order;
    }

    //O(1) deallocation:Memory allocated with os and freelist got empty
    void deallocate(Order* order){
        freeList.push(order);
    }
};