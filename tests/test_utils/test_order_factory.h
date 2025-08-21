#include "../../orderbook/order.h"
#include <memory>

namespace exch_ns
{
class TestOrderFactory
{
  public:
    static Order* Create(Price price, Quantity qty, Side side, SequenceNumber seq_no)
    {
        Order* ord = new Order();
        ord->quantity_    = qty;
        ord->side_        = side;
        ord->price_       = price;
        ord->exch_seq_no_ = seq_no;
        return ord;
    }
};
} // namespace exch_ns