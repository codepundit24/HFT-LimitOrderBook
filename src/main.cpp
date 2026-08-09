#include <iostream>
#include <chrono>
#include "OrderBook.h"
#include "OrderPool.h"

int main(){
    const int NUM_ORDERS = 1000000;
    OrderPool pool(NUM_ORDERS +10);
    OrderBook book;

    std::cout<<"--- HFT Engine Performance Benchmark ---"<<std::endl;
    std::cout<<" Generating and matching "<<NUM_ORDERS<<" orders..." <<std::endl;
    std::cout<<"-Please wait..."<<std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i=0; i < NUM_ORDERS; i++){
        OrderType side = (i % 2 ==0) ? OrderType::BUY : OrderType::SELL;

        double price = 150 + (i % 5);

        uint32_t qty = 10 + (i % 100);

        Order* order = pool.allocate(i + 1,side, price, qty,i);
        book.addOrder(order);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    double seconds = duration.count() / 1000;

    std::cout<<"\n>>> Benchmark Complete! <<<" <<std::endl;
    std::cout<<"Total time taken: "<< duration.count() << "ms ("<<seconds<<"seconds)" <<std::endl;

    if(seconds >0){
        std::cout<<"Speed: "<<(NUM_ORDERS /seconds) <<" Orders per seconds! "<<std::endl;
    }

    return 0;
}