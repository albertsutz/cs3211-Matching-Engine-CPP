#include "result.hpp" 


Deleted::Deleted(int32_t id, 
    bool cancel_accepted, 
    intmax_t output_timestamp):
    id{id}, 
    cancel_accepted{cancel_accepted}, 
    output_timestamp{output_timestamp}
{
}

ResultType Deleted::get_type() 
{
    return ORDER_DELETED; 
}

Added::Added(uint32_t id, 
        const char* symbol,
        uint32_t price, 
        uint32_t count, 
        bool is_sell_side, 
        intmax_t output_timestamp): 
        id{id}, 
        symbol{symbol},
        count{count}, 
        is_sell_side{is_sell_side},
        output_timestamp{output_timestamp}
        {
        }

ResultType Added::get_type() 
{
    return ORDER_ADDED; 
}

Executed::Executed(uint32_t resting_id,
	    uint32_t new_id,
	    uint32_t execution_id,
	    uint32_t price,
	    uint32_t count,
	    intmax_t output_timestamp):
        resting_id{resting_id},
        new_id{new_id},
        execution_id{execution_id},
        price{price},
        count{count},
        output_timestamp{output_timestamp}
        {
        }

ResultType Executed::get_type() 
{
    return ORDER_EXECUTED; 
}