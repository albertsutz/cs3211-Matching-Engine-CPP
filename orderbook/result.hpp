#pragma once 

#include <unordered_map>
#include <vector> 

enum ResultType {ORDER_ADDED, ORDER_EXECUTED, ORDER_DELETED};

class ResultWrapper {
public:  
    ResultWrapper();
    void add_result(IResult*); 
    void add_deleted(uint32_t); 
    std::vector<IResult*> get_result(); 
    std::vector<uint32_t> get_deleted_ids(); 
    bool is_added(); 
    void set_added();

private:
    std::vector<IResult*> results; 
    std::vector<uint32_t> deleted_ids; 
    bool added;
};

class IResult 
{
public:
    virtual ResultType get_type() = 0;
};

class Deleted: public IResult 
{
public:
    Deleted(int32_t id, 
        bool cancel_accepted, 
        intmax_t output_timestamp);
    virtual ResultType get_type();

    uint32_t id;         
    bool cancel_accepted;              
    intmax_t output_timestamp;   
};

class Added: public IResult 
{
public:
    Added(uint32_t id, 
        const char* symbol, 
        uint32_t price, 
        uint32_t count, 
        bool is_sell_side, 
        intmax_t output_timestamp);
    virtual ResultType get_type();

    uint32_t id; 
    const char* symbol; 
    uint32_t price;
    uint32_t count;
    bool is_sell_side;
    intmax_t output_timestamp;
};

class Executed: public IResult
{
public:
    Executed(uint32_t resting_id,
	    uint32_t new_id,
	    uint32_t execution_id,
	    uint32_t price,
	    uint32_t count,
	    intmax_t output_timestamp);
    virtual ResultType get_type();

     uint32_t resting_id;
    uint32_t new_id;
    uint32_t execution_id;
    uint32_t price;
    uint32_t count;
    intmax_t output_timestamp;   
};
