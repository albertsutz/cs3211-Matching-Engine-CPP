#pragma once 

#include <set> 
#include "order.hpp" 
#include "result.hpp" 
#include <vector>
#include <set>

auto buyComparator = [](Order a, Order b) {
    if(a.price != b.price) {
        return a.price - b.price;
    } else {
        return b.time - a.time;
    }
};

auto sellComparator = [](Order a, Order b) {
    if(a.price != b.price) {
        return b.price - a.price;
    } else {
        return b.time - a.time;
    }
};

class Instrument {
public: 
    std::set<Order, decltype(buyComparator)> buySet;
    std::set<Order, decltype(sellComparator)> sellSet;

    ResultWrapper process_order(Order); 
    ResultWrapper process_cancel(CancelOrder, OrderType); 
    ResultWrapper process_buy(Order);
    ResultWrapper process_sell(Order);

    Instrument();
private:
    
};

inline std::chrono::microseconds::rep getCurrentTimestamp() noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
