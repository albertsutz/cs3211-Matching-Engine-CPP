#include "instrument.hpp"
#include <set>
#include <iostream>
#include <memory>

Instrument :: Instrument() = default;

ResultWrapper Instrument :: process_order(Order order) {
    if(order.order_type == OrderType::BUY) {
        // std::cout << "BUY" << std::endl;
        return process_buy(order);
    } else {
        // std::cout << "SELL" << std::endl;
        return process_sell(order);
    }
}

ResultWrapper Instrument :: process_cancel(CancelOrder cancel_order, OrderType type) {
    std::unique_lock instrument_lock {instr_mutex};
    bool deleted = false;
    if (type == OrderType :: BUY) {
        for(auto i = buySet.begin(), last = buySet.end(); i != last; ) {
            if((*i).order_id == cancel_order.order_id) {
                buySet.erase(i);
                deleted = true;
                break;
            }
        }
    } else {
        for(auto i = sellSet.begin(), last = sellSet.end(); i != last; ) {
            if((*i).order_id == cancel_order.order_id) {
                sellSet.erase(i);
                deleted = true;
                break;
            }
        }
    }
    
    if(deleted) {
        ResultWrapper res; 
        res.add_result(std::make_shared<Deleted>(
            cancel_order.order_id, true, getCurrentTimestamp()
        ));
        res.add_deleted(cancel_order.order_id);
        return res;
    } else {
        ResultWrapper res; 
        res.add_result(std::make_shared<Deleted>(
            cancel_order.order_id, false, getCurrentTimestamp()
        ));
        return res;
    }

}

ResultWrapper Instrument :: execute_buy(Order buyOrder) {
    ResultWrapper result;
    // cmn bsa 1 yang di execution
    std::unique_lock execution_lock {execution_mutex};
    while (true) {
        std::unique_lock sell_lock {sell_mutex};
        if(sellSet.empty()) {
            break;
        }
        //not empty, check can match or not
        // i have exclusive pop power anyways
        Order bestSell = *(std::prev(sellSet.end()));
        sell_lock.unlock();

        if(buyOrder.price >= bestSell.price) {
            uint32_t quantityTraded = std::min(buyOrder.count, bestSell.count);
            bestSell.count -= quantityTraded;
            buyOrder.count -= quantityTraded;
            result.add_result(std::make_shared<Executed>(bestSell.order_id, buyOrder.order_id,
	            bestSell.execution_id++, bestSell.price, quantityTraded, getCurrentTimestamp()
                ));

            sell_mutex.lock();
            sellSet.erase(std::prev(sellSet.end()));
            if(bestSell.count > 0) {
                sellSet.insert(bestSell);
                break;
            } else if (buyOrder.count > 0) {
                result.add_deleted(bestSell.order_id);
                sell_mutex.unlock();
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
        std::unique_lock buy_lock {buy_mutex};
        buySet.insert(buyOrder);
        result.add_result(std::make_shared<Added>(buyOrder.order_id, buyOrder.instrument, buyOrder.price,
            buyOrder.count, buyOrder.order_type == OrderType::SELL, getCurrentTimestamp()));
        result.set_added();
    }
    return result; 
}

ResultWrapper Instrument :: execute_sell(Order sellOrder) {
    ResultWrapper result;
    // cmn bsa 1 yang di execution
    std::unique_lock execution_lock {execution_mutex};
    while (true) {
        std::unique_lock buy_lock {buy_mutex};
        if(buySet.empty()) {
            break;
        }
        //not empty, check can match or not
        // i have exclusive pop power anyways
        Order bestBuy = *(std::prev(buySet.end()));
        buy_lock.unlock();

        if(bestBuy.price >= sellOrder.price) {
            uint32_t quantityTraded = std::min(bestBuy.count, sellOrder.count);
            bestBuy.count -= quantityTraded;
            sellOrder.count -= quantityTraded;
            result.add_result(std::make_shared<Executed>(bestBuy.order_id, sellOrder.order_id,
	            bestBuy.execution_id++, bestBuy.price, quantityTraded, getCurrentTimestamp()
                ));

            buy_lock.lock();
            buySet.erase(std::prev(buySet.end()));
            if(bestBuy.count > 0) {
                buySet.insert(bestBuy);
                break;
            } else if (sellOrder.count > 0) {
                result.add_deleted(bestBuy.order_id);
                buy_mutex.unlock();
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
        std::unique_lock sell_lock {sell_mutex};
        sellSet.insert(sellOrder);
        result.add_result(std::make_shared<Added>(sellOrder.order_id, sellOrder.instrument, sellOrder.price,
            sellOrder.count, sellOrder.order_type == OrderType::SELL, getCurrentTimestamp()));
        result.set_added();
    }
    return result; 
}

ResultWrapper Instrument :: process_buy(Order buyOrder) {
    std::unique_lock sell_lock {sell_mutex};
    if(sellSet.empty()) {
        return right?;
    } 
    //not empty, acquire the top one
    Order bestSell = *(std::prev(sellSet.end()));
    sell_lock.unlock();

    if(buyOrder.price >= bestSell.price) {
        //match, we execute
        return execute_buy(buyOrder);
    } else {
        //masukkin aja
        ResultWrapper result;
        std::unique_lock buy_lock {buy_mutex};
        buySet.insert(buyOrder);
        result.add_result(std::make_shared<Added>(buyOrder.order_id, buyOrder.instrument, buyOrder.price,
            buyOrder.count, buyOrder.order_type == OrderType::SELL, getCurrentTimestamp()));
        result.set_added();
        return result;
    } 
}

ResultWrapper Instrument :: process_sell(Order sellOrder) {
    std::unique_lock buy_lock {buy_mutex};
    if(buySet.empty()) {
        return right?;
    } 
    //not empty, acquire the top one
    Order bestBuy = *(std::prev(buySet.end()));
    buy_lock.unlock();

    if(bestBuy.price >= sellOrder.price) {
        //match, we execute
        return execute_sell(sellOrder);
    } else {
        //masukkin aja
        ResultWrapper result;
        std::unique_lock sell_lock {sell_mutex};
        sellSet.insert(sellOrder);
        result.add_result(std::make_shared<Added>(sellOrder.order_id, sellOrder.instrument, sellOrder.price,
            sellOrder.count, sellOrder.order_type == OrderType::SELL, getCurrentTimestamp()));
        result.set_added();
        return result;
    } 
}

// ResultWrapper Instrument :: process_sell(Order sellOrder) {
//     ResultWrapper result;

//     while(!buySet.empty()) {
//         // get the last buy
//         Order bestBuy = *(std::prev(buySet.end()));
//         // pop the last buy
//         if(bestBuy.price >= sellOrder.price) {
//             uint32_t quantityTraded = std::min(bestBuy.count, sellOrder.count);
//             bestBuy.count -= quantityTraded;
//             sellOrder.count -= quantityTraded;
//             result.add_result(std::make_shared<Executed>(bestBuy.order_id, sellOrder.order_id,
// 	            bestBuy.execution_id++, bestBuy.price, quantityTraded, getCurrentTimestamp()
//                 ));

//             buySet.erase(std::prev(buySet.end()));
//             if(bestBuy.count > 0) {
//                 buySet.insert(bestBuy);
//                 break;
//             } else if (sellOrder.count > 0) {
//                 result.add_deleted(bestBuy.order_id);
//                 continue;
//             } else {
//                 result.add_deleted(bestBuy.order_id);
//                 break;
//             }
//         } else {
//             break;
//         }
//     }
//     if(sellOrder.count > 0) {
//         sellSet.insert(sellOrder);
//         result.add_result(std::make_shared<Added>(sellOrder.order_id, sellOrder.instrument, sellOrder.price,
//             sellOrder.count, sellOrder.order_type == OrderType::SELL, getCurrentTimestamp()));
//         result.set_added();
//     }
//     return result;
// }