#include "orderbook.hpp" 


ResultWrapper Orderbook::process_order(Order order) 
{
    if (!is_exist_instr(order.instrument)) {
        m_instrument_map.insert({order.instrument, Instrument()});
    }
    auto instr = m_instrument_map.at(order.instrument); 
    auto res = instr.process_order(order); 

    if (res.is_added()) {
        // need to check which ids have been deleted as well 
        m_id_to_instrument[order.order_id] = order.instrument;
    }
    return res;
}

ResultWrapper Orderbook::process_cancel(CancelOrder order) 
{   
    if (!is_exist_id(order.order_id)) {
        // return cancel fails 
    }

    auto key = m_id_to_instrument.at(order.order_id); 
    auto instr = m_instrument_map.at(key);
    auto res = instr.process_cancel(order); 

    // check if deleted 
}

bool Orderbook::is_exist_instr(std::string instr) 
{
    return m_instrument_map.find(instr) != m_instrument_map.end(); 
}
