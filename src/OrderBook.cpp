#include "OrderBook.h"

void OrderBook::addOrder(Order* order){
    if(order->side == OrderType::BUY){
        bids[order->price] = order;
    }else{
        asks[order->price] = order;
    }
    orderLookup[order->orderId] = order;

    //After new order, matching logic triggered
    matchOrders();
}

void OrderBook::matchOrders(){
    while(!bids.empty() && !asks.empty()){
        auto bestBid = bids.begin();
        auto bestAsk = asks.begin();

        //Check if Buy Price >= Sell Price
        if(bestBid->first >= bestAsk->first){
            Order* buyOrder = bestBid->second;
            Order* sellOrder = bestAsk->second;

            uint32_t tradedQty = std::min(buyOrder->quantity,sellOrder->quantity);

            std::cout << "\n>>> MATCH EXECUTED <<<" << std::endl;
            std::cout << "Traded Quantity: "<< tradedQty
                      << " @ price: $" << sellOrder->price << std::endl;
            
            buyOrder->quantity -= tradedQty;
            sellOrder->quantity -= tradedQty;

            //Fill status hanlde 
            if(buyOrder->quantity == 0){
                bids.erase(bestBid);
            }
            if(sellOrder->quantity== 0){
                asks.erase(bestAsk);
            }
        }else{
            break; // Matching price can't found
        }
    }
}

void OrderBook::printBook() const{
    std::cout << "\n--- CURRENT ORDER BOOK ---"<< std::endl;
    std::cout << "ASKS (Sell Side):"<< std::endl;
    for (const auto& [price,order] : asks){
        std::cout << "  $"<< price<<" | Qty: "<<order->quantity <<std::endl;
    }
    std::cout <<" BIDS (Buy Side):"<<std::endl;
    for (const auto& [price,order] : bids){
        std::cout<<"  $"<<price<<" | Qty: "<<order->quantity << std::endl;
    }
}