#pragma once 

#include <set> 
#include "order.hpp" 
#include "result.hpp" 
#include <vector>
#include <set>
#include <iostream>
#include <mutex>

auto buyComparator = [](Order a, Order b) {
    if(a.price != b.price) {
        return a.price < b.price;
    } else {
        return b.time < a.time;
    }
};

auto sellComparator = [](Order a, Order b) {
    if(a.price != b.price) {
        return b.price < a.price;
    } else {
        return b.time < a.time;
    }
};

class Instrument {
public: 
    std::set<Order, decltype(buyComparator)> buySet;
    std::set<Order, decltype(sellComparator)> sellSet;
    std::mutex m_instrument_mutex;
    // std::mutex buy_mutex;
    // std::mutex sell_mutex;
    // std::mutex buy_set_mutex;
    // std::mutex sell_set_mutex;
    // std::mutex execution_mutex;

    ResultWrapper process_order(Order); 
    ResultWrapper process_cancel(CancelOrder, OrderType); 
    ResultWrapper process_buy(Order);
    ResultWrapper process_sell(Order);
    ResultWrapper execute_buy(Order);
    ResultWrapper execute_sell(Order);

    Instrument();

    void printSellSet() {
        for(auto i = sellSet.begin(); i != sellSet.end(); i++) {
            std::cout << (*i).price << ' ';
        }
        std::cout << std::endl;
    }
    void printBuySet() {
        for(auto i = buySet.begin(); i != buySet.end(); i++) {
            std::cout << (*i).price << ' ';
        }
        std::cout << std::endl;
    }
private:
    
};

inline std::chrono::microseconds::rep getCurrentTimestamp() noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
