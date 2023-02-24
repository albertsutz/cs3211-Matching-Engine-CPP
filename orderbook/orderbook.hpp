#pragma once 

#include <unordered_map> 
#include <string> 
#include "instrument.hpp"
#include "order.hpp" 
#include "result.hpp"
#include <vector> 

class Orderbook 
{
public: 
    std::vector<IResult> process_order(Order);  
    std::vector<IResult> process_cancel(CancelOrder); 

private:
    std::unordered_map<std::string, Instrument> m_instrument_map; 
    bool is_exist(std::string); 
};