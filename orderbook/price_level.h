#pragma once
#include "./order.h"
#include "fifo_order_queue.h"

namespace exch_ns
{
class PriceLevel
{
  public:
    PriceLevel()
    {
    }

    inline void Add(Order &order)
    {
        order_queue_.Enqueue(&order);
        total_quantity_ += order.GetQuantity();
    }

    inline void AddStop(Order &order)
    {
        trigger_awaiting_orders_.Enqueue(&order);
    }

    inline Quantity GetTotalQuantity() const
    {
        return total_quantity_;
    }

    void OnTrigger()
    {
        /* disclose all trigger awaiting orders and try to match them according to their type. */
    }

  private:
    FifoOrderQueue order_queue_;
    FifoOrderQueue trigger_awaiting_orders_;
    /* Add another queue for trigger awaiting orders at this price */
    Quantity total_quantity_ = Quantity(0);
};
}