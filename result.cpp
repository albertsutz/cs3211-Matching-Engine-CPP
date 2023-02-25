#include "result.hpp" 


std::vector<IResult*> ResultWrapper::get_result() 
{
    return results; 
}

bool ResultWrapper::is_added() 
{
    return added; 
}

ResultWrapper::ResultWrapper():
    added{false} 
{ 
}

void ResultWrapper::add_result(IResult* res) 
{
    results.push_back(res); 
}

void ResultWrapper::set_added() {
    added = true;
}

void ResultWrapper::add_deleted(uint32_t id) {
    deleted_ids.push_back(id);
}

std::vector<uint32_t> ResultWrapper::get_deleted_ids() {
    return deleted_ids;
}

Deleted::Deleted(uint32_t id, 
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

Added::Added(
    uint32_t id, 
        std::string symbol,
        uint32_t price, 
        uint32_t count, 
        bool is_sell_side, 
        intmax_t output_timestamp): 
        id{id}, 
        symbol{symbol},
        price{price},
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