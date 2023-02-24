#pragma once 

#include <cstdint>


enum OrderType {BUY, SELL};

struct Order 
{
    OrderType order_type;  
    uint32_t order_id; 
    uint32_t price; 
    uint32_t count; 
    char instrument[9]; 
};

struct CancelOrder 
{
    uint32_t order_id; 
};
