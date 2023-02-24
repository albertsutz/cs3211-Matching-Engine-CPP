#include "orderbook.hpp" 


std::vector<IResult> Orderbook::process_order(Order order) 
{
    if (!is_exist(order.instrument)) {
        m_instrument_map.insert({order.instrument, Instrument()});
    }
    auto instr = m_instrument_map.at(order.instrument); 
    auto res = instr.process_order(order); 
    // Logic -> if added to the order book, add to id to instrument mapping 
}

std::vector<IResult> Orderbook::process_cancel(CancelOrder order) 
{   
    
}

bool Orderbook::is_exist(std::string instr) 
{
    return m_instrument_map.find(instr) != m_instrument_map.end(); 
}
