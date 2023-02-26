#include "orderbook.hpp" 
#include <memory>

ResultWrapper Orderbook::process_order(Order order) 
{
    std::unique_lock global_lock {m_global_mutex};
    if (!is_exist_instr(order.instrument)) {
        m_instrument_map.emplace(std::piecewise_construct, std::forward_as_tuple(order.instrument), std::forward_as_tuple());
    }
    auto& instruction_object = m_instrument_map.at(order.instrument);
    m_id_map[order.order_id] = std::make_pair(order.instrument, order.order_type);
    global_lock.unlock();

    instruction_object.enter(order.order_type);
    auto result = instruction_object.process_order(order); 
    instruction_object.exit(order.order_type);

    return result;
}

ResultWrapper Orderbook::process_cancel(CancelOrder order) 
{   
    //ambil mutex disini aja 
    std::unique_lock global_lock {m_global_mutex};
    auto pair_name_type = m_id_map.at(order.order_id); 
    auto& instruction_object = m_instrument_map.at(pair_name_type.first);
    global_lock.unlock();

    instruction_object.enter(CANCEL);
    auto res = instruction_object.process_cancel(order, pair_name_type.second); 
    instruction_object.exit(CANCEL);

    return res; 
}

bool Orderbook::is_exist_instr(std::string instr) 
{
    return m_instrument_map.find(instr) != m_instrument_map.end(); 
}

bool Orderbook::is_exist_id(uint32_t id) 
{
    return m_id_map.find(id) != m_id_map.end();
}
