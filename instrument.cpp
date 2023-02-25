#include "instrument.hpp"
#include <set>

Instrument :: Instrument() = default;

ResultWrapper Instrument :: process_order(Order order) {
    if(order.order_type == OrderType::BUY) {
        return process_buy(order);
    } else {
        return process_sell(order);
    }
}

ResultWrapper Instrument :: process_cancel(CancelOrder cancel_order, OrderType type) {
    if (type == OrderType :: BUY) {
        for(auto i = buySet.begin(), last = buySet.end(); i != last; ) {
            if((*i).order_id == cancel_order.order_id) {
                buySet.erase(i);
                break;
            }
        }
    } else {
        for(auto i = sellSet.begin(), last = sellSet.end(); i != last; ) {
            if((*i).order_id == cancel_order.order_id) {
                sellSet.erase(i);
                break;
            }
        }
    }
    
    ResultWrapper res; 
    res.add_result(new Deleted(
        cancel_order.order_id, true, getCurrentTimestamp()
    ));
    res.add_deleted(cancel_order.order_id);
    return res;
}

ResultWrapper Instrument :: process_buy(Order buyOrder) {
    ResultWrapper result;

    while(!sellSet.empty()) {
        // get the last sell (copy?)
        Order bestSell = *(std::prev(sellSet.end()));
        //pop the last
        sellSet.erase(std::prev(sellSet.end()));
        if(buyOrder.price >= bestSell.price) {
            uint32_t quantityTraded = std::min(buyOrder.count, bestSell.count);
            bestSell.count -= quantityTraded;
            buyOrder.count -= quantityTraded;

            result.add_result(new Executed(bestSell.order_id, buyOrder.order_id,
	            bestSell.execution_id++, bestSell.price, quantityTraded, getCurrentTimestamp()
                ));
                
            if(bestSell.count > 0) {
                sellSet.insert(bestSell);
                break;
            } else if (buyOrder.count > 0) {
                result.add_deleted(bestSell.order_id);
                continue;
            } else {
                result.add_deleted(bestSell.order_id);
                break;
            }
        } else {
            break;
        }
    }
    if(buyOrder.count > 0) {
        buySet.insert(buyOrder);
        result.add_result(new Added(buyOrder.order_id, buyOrder.instrument, buyOrder.price,
            buyOrder.count, buyOrder.order_type == OrderType::BUY, getCurrentTimestamp()));
        result.set_added();
    }
    return result; 
}

ResultWrapper Instrument :: process_sell(Order sellOrder) {
    ResultWrapper result;

    while(!buySet.empty()) {
        // get the last buy
        Order bestBuy = *(std::prev(buySet.end()));
        // pop the last buy
        buySet.erase(std::prev(buySet.end()));
        if(sellOrder.price >= bestBuy.price) {
            uint32_t quantityTraded = std::min(bestBuy.count, sellOrder.count);
            bestBuy.count -= quantityTraded;
            sellOrder.count -= quantityTraded;
            result.add_result(new Executed(bestBuy.order_id, sellOrder.order_id,
	            bestBuy.execution_id++, bestBuy.price, quantityTraded, getCurrentTimestamp()
                ));
            if(bestBuy.count > 0) {
                buySet.insert(bestBuy);
                break;
            } else if (sellOrder.count > 0) {
                result.add_deleted(bestBuy.order_id);
                continue;
            } else {
                result.add_deleted(bestBuy.order_id);
                break;
            }
        } else {
            break;
        }
    }
    if(sellOrder.count > 0) {
        sellSet.insert(sellOrder);
        result.add_result(new Added(sellOrder.order_id, sellOrder.instrument, sellOrder.price,
            sellOrder.count, sellOrder.order_type == OrderType::SELL, getCurrentTimestamp()));
        result.set_added();
    }
    return result;
}