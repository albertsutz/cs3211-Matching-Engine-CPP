#include <cstdint>

class Order {
public:
    uint32_t order_id;
    uint32_t price;
    uint32_t count;
    uint32_t time;

    Order(uint32_t order_id, uint32_t price, uint32_t count, uint32_t time)
        : order_id(order_id), price(price), count(count), time(time) { }
    // int Compare(const Order& other);
    // bool operator == (const Order& other);
    // bool operator < (const Order& other);
    // bool operator > (const Order& other);
};