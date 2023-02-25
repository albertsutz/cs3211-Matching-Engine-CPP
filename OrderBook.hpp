#include <unordered_map>
#include <string>
#include "InstrumentBook.hpp"

class OrderBook {
public:
    std::unordered_map<std::string, InstrumentBook> instruments;
    OrderBook();
};