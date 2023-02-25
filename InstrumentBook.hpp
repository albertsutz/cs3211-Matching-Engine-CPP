#include <set>
#include "Order.hpp"

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

class InstrumentBook {
public:
    std::set<Order, decltype(buyComparator)> buySet;
    std::set<Order, decltype(sellComparator)> sellSet;

    void processBuy(Order buyOrder);
    void processSell(Order sellOrder);

    InstrumentBook();
};



