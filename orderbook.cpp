#include "orderbook.hpp" 
#include <memory>

ResultWrapper Orderbook::process_order(Order order) 
{
    if (!is_exist_instr(order.instrument)) {
        m_instrument_map.insert({order.instrument, Instrument()});
    }
    auto& instruction_object = m_instrument_map.at(order.instrument); 
    auto result = instruction_object.process_order(order); 

    if (result.is_added()) {
        m_id_map[order.order_id] = std::make_pair(order.instrument, order.order_type);
    }

    for (auto& id: result.get_deleted_ids()) {
        m_id_map.erase(id);
    }

    return result;
}

ResultWrapper Orderbook::process_cancel(CancelOrder order) 
{   
    //ambil mutex disini aja 
    if (!is_exist_id(order.order_id)) {
        ResultWrapper result; 
        result.add_result(std::make_shared<Deleted>(
            order.order_id, false, getCurrentTimestamp()
        ));
        return result; 
    }

    auto pair_name_type = m_id_map.at(order.order_id); 
    auto& instruction_object = m_instrument_map.at(pair_name_type.first);
    m_id_map.erase(order.order_id);
    return instruction_object.process_cancel(order, pair_name_type.second); 
}

bool Orderbook::is_exist_instr(std::string instr) 
{
    return m_instrument_map.find(instr) != m_instrument_map.end(); 
}

bool Orderbook::is_exist_id(uint32_t id) 
{
    return m_id_map.find(id) != m_id_map.end();
}
