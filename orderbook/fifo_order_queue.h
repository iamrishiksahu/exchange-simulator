#pragma once
#include "order.h"
#include <unordered_map>

namespace exch_ns
{
class FifoOrderQueue
{
  public:
    inline ErrorCode Enqueue(Order *order)
    {
        if (last_ != nullptr)
        {
            auto ord = const_cast<Order *>(last_);
            ord->SetNextOrder(order);
        }
        else
        {
            first_ = order;
            last_  = order;
        }

        return ErrorCode::NoError;
    }

    inline ErrorCode Dequeue(Order *order)
    {
        auto itr = order_location_.find(order->GetOrderId());
        if (itr != order_location_.end())
        {
            itr->second->SetPrevOrder(itr->second->GetNextOrder());
            return ErrorCode::NoError;
        }
        return ErrorCode::CouldNotFindOrder;
    }

  private:
    std::unordered_map<OrderId, Order *> order_location_;
    Order                               *first_ = nullptr;
    Order                               *last_  = nullptr;
};
}