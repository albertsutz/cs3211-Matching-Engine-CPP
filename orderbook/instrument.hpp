#pragma once 

#include <set> 
#include "order.hpp" 
#include "result.hpp" 
#include <vector>

class Instrument {
public: 
    ResultWrapper process_order(Order); 
    ResultWrapper process_cancel(CancelOrder); 

private:
    
};