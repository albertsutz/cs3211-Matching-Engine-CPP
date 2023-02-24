#pragma once 

#include <unordered_map>

enum ResultType {ORDER_ADDED, ORDER_EXECUTED, ORDER_DELETED};

class IResult 
{
public:
    virtual ResultType get_type() = 0;
};

class Deleted: IResult 
{
public:
    Deleted(int32_t id, 
        bool cancel_accepted, 
        intmax_t output_timestamp);
    virtual ResultType get_type();

private:
    uint32_t id;         
    bool cancel_accepted;              
    intmax_t output_timestamp;                 
};

class Added: IResult 
{
public:
    Added(uint32_t id, 
        const char* symbol, 
        uint32_t price, 
        uint32_t count, 
        bool is_sell_side, 
        intmax_t output_timestamp);
    virtual ResultType get_type();

private:
uint32_t id; 
    const char* symbol; 
    uint32_t price;
    uint32_t count;
    bool is_sell_side;
    intmax_t output_timestamp;
};

class Executed: IResult
{
public:
    Executed(uint32_t resting_id,
	    uint32_t new_id,
	    uint32_t execution_id,
	    uint32_t price,
	    uint32_t count,
	    intmax_t output_timestamp);
    virtual ResultType get_type();

private:
    uint32_t resting_id;
    uint32_t new_id;
    uint32_t execution_id;
    uint32_t price;
    uint32_t count;
    intmax_t output_timestamp;
};

