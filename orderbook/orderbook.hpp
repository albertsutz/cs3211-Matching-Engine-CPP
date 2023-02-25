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
    ResultWrapper process_order(Order);  
    ResultWrapper process_cancel(CancelOrder); 

private:
    std::unordered_map<std::string, Instrument> m_instrument_map; 
    std::unordered_map<uint32_t, std::string> m_id_to_instrument; 
    bool is_exist_instr(std::string); 
    bool is_exist_id(uint32_t); 
};