#include "InstrumentBook.hpp"


InstrumentBook :: InstrumentBook() = default;

void InstrumentBook :: processBuy(Order buyOrder) {
    while(!sellSet.empty()) {
        Order bestSell = *(std::prev(sellSet.end()));
        if(buyOrder.price >= bestSell.price) {
            uint32_t quantityTraded = std::min(buyOrder.count, bestSell.count);
            bestSell.count -= quantityTraded;
            buyOrder.count -= quantityTraded;
            if(bestSell.count > 0) {
                break;
            } else if (buyOrder.count > 0) {
                sellSet.erase(std::prev(sellSet.end()));
                continue;
            } else {
                sellSet.erase(std::prev(sellSet.end()));
                break;
            }
        } else {
            break;
        }
    }
    if(buyOrder.count > 0) {
        buySet.insert(buyOrder);
    }
}