#ifdef UNIT_TESTING
class OrderFactoryForTest {
public:
    static Order* Create(OrderId id, Price price, Quantity qty) {
        return new Order(id, price, qty);
    }
};
#endif
