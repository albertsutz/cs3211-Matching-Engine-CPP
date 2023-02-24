#pragma once 

#include <set> 
#include "order.hpp" 
#include "result.hpp" 
#include <vector>

class Instrument {
public: 
    std::vector<IResult> process_order(Order); 
    std::vector<IResult> process_cancel(CancelOrder); 

private:
    
};