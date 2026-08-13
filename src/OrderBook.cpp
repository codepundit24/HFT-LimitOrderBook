#include "OrderBook.h"

void OrderBook::addOrder(Order* order){
    //Put the order in lookup for fast cancel 
    orderLookup[order->orderId] = order;

    if (order->type == OrderType::STOP_LOSS){
        stopLossOrders.push_back(order);
        std::cout<<"Stop-Loss Order ID "<<order->orderId<< " added to pending list. \n";
        return;
    }
    //Keep the order in it's price queue at last
    if(order->side == Side::BUY){
        bids[order->price].append(order);
    }else{
        asks[order->price].append(order);
    }

    //After new order, matching logic triggered
    matchOrders();
}


void OrderBook::checkStopLossTriggers(double lastTradePrice){
    //Iterate through pending stop-loss orders
    for (auto it = stopLossOrders.begin(); it != stopLossOrders.end();){
        Order* order = *it;
        bool triggered = false;

        if (order->side == Side::BUY && lastTradePrice >= order->trigger_price){
            triggered = true;
        }
        else if (order->side == Side::SELL && lastTradePrice <= order->trigger_price){
            triggered = true;
        }
        if (triggered){
            std::cout << "Stop-Loss Triggered for Order ID "<< order->orderId
                      << " at market price "<<lastTradePrice<< "!\n";
            
            order->type = OrderType::LIMIT;
            addOrder(order);

            it = stopLossOrders.erase(it);
        }else ++it;
    }
}
void OrderBook::cancelOrder(uint64_t orderId){
    //Check if order is exist or not
    auto it = orderLookup.find(orderId);
    if(it == orderLookup.end()){
        // std::cout<< "Order ID "<<orderId<<" not found for cancellation."<<std::endl;
        return;
    }

    Order* order = it->second;

    //Take out the order from it's doubly linked list 
    if(order->side == Side::BUY){
        bids[order->price].remove(order);
        if(bids[order->price].isEmpty()){
            bids.erase(order->price);
        }
    }else{
        asks[order->price].remove(order);
        if(asks[order->price].isEmpty()){
            asks.erase(order->price);
        }
    }

    //remove the lookup
    orderLookup.erase(orderId);

    // std::cout<<">>> ORDER CANCELLED: ID "<<orderId<<" <<<"<<std::endl;
}


void OrderBook::matchOrders(){
    while(!bids.empty() && !asks.empty()){
        auto bestBidIt = bids.begin();
        auto bestAskIt = asks.begin();

        double bestBidPrice = bestBidIt->first;
        double bestAskPrice = bestAskIt->first;

        //Check if Buy Price >= Sell Price
        if(bestBidPrice >= bestAskPrice){
            OrderList& bidQueue = bestBidIt->second;
            OrderList& askQueue = bestAskIt->second;

            Order* buyOrder = bidQueue.head;
            Order* sellOrder = askQueue.head;

            uint32_t tradedQty = std::min(buyOrder->quantity,sellOrder->quantity);

            // std::cout << "\n>>> MATCH EXECUTED <<<" << std::endl;
            // std::cout << "Traded Quantity: "<< tradedQty
            //           << " @ price: $" << sellOrder->price
            //           << " (Buyer ID: "<<buyOrder->orderId
            //           << ", Seller ID: "<<sellOrder->orderId<<")" << std::endl;
            
            double executionPrice  = sellOrder->price;

            checkStopLossTriggers(executionPrice);
            
            buyOrder->quantity -= tradedQty;
            sellOrder->quantity -= tradedQty;

            //Fill status hanlde 
            if(buyOrder->quantity == 0){
                bidQueue.remove(buyOrder);
                orderLookup.erase(buyOrder->orderId);
                if(bidQueue.isEmpty()) bids.erase(bestBidIt);
            }
            if(sellOrder->quantity== 0){
                askQueue.remove(sellOrder);
                orderLookup.erase(sellOrder->orderId);
                if(askQueue.isEmpty()) asks.erase(bestAskIt);
            }
        }else{
            break; // Matching price can't found
        }
    }
}

void OrderBook::printBook() const{
    std::cout << "\n--- CURRENT ORDER BOOK ---"<< std::endl;
    std::cout << "ASKS (Sell Side):"<< std::endl;
    for (auto it=asks.rbegin(); it != asks.rend(); ++it){
        Order* current = it->second.head;
        std::cout<<"  $"<<it->first<<" -> [ ";
        while (current !=nullptr){
            std::cout<<"(ID: "<<current->orderId<<" Qty:"<<current->quantity<<") ";
            current = current->next;
        }
        std::cout<<"]"<<std::endl;
    }
    std::cout <<" BIDS (Buy Side):"<<std::endl;
    for (const auto& [price,orderList] : bids){
        Order* current = orderList.head;
        std::cout<<"  $"<<price<<" => [ ";
        while (current != nullptr)
        {
            std::cout<<"(ID:"<<current->orderId<<"Qty:"<<current->quantity<<") ";
            current = current->next;
        }
        std::cout<<"]"<<std::endl;
    }
    std::cout<<"-----------------------------\n"<<std::endl;
}