#pragma once 

#include <cstdint>


enum OrderType {BUY, SELL};

struct Order 
{
    OrderType order_type;  
    uint32_t order_id; 
    uint32_t price; 
    uint32_t count; 
    uint32_t time;
    std::string instrument; 
    uint32_t execution_id;

    // Order(OrderType order_type, uint32_t order_id, uint32_t price, uint32_t count, uint32_t time, char instrument[9])
    //     : order_type{order_type}, order_id{order_id}, price{price}, count{count}, time{time}, instrument{instrument} { }
};

struct CancelOrder 
{
    uint32_t order_id; 

    CancelOrder(uint32_t order_id)
        : order_id{order_id} { }
};
