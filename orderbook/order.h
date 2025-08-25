#pragma once
#include "../common/order_enums.h"
#include "types.h"
#include <iostream>
#include <optional>

namespace exch_ns
{
class TestOrderFactory;
class Order
{
    friend class TestOrderFactory;

  public:
    // Order() = default;
    inline const std::optional<Price> &GetLimitPrice() const noexcept
    {
        return price_;
    }

    inline const std::optional<Price> &GetStopTriggerPrice() const noexcept
    {
        return stop_trigger_price_;
    }

    inline const Quantity GetQuantity() const noexcept
    {
        return quantity_;
    }

    inline const OrderId GetOrderId() const noexcept
    {
        return id_;
    }

    inline const Side GetSide() const noexcept
    {
        return side_;
    }

    inline const OrderType GetType() const noexcept
    {
        return order_type_;
    }

    inline void SetNextOrder(Order *order) noexcept
    {
        next_ = order;
    }

    inline Order *GetNextOrder() const noexcept
    {
        return next_;
    }

    inline void SetPrevOrder(Order *order) noexcept
    {
        prev_ = order;
    }

  private:
    Order() = default;

  private:
    OrderId              id_;
    Quantity             quantity_;
    std::optional<Price> price_;
    std::optional<Price> stop_trigger_price_;
    Side                 side_;
    OrderId              parent_id_;
    Timestamp            exch_recv_time_;
    Timestamp            client_sent_time_;
    SequenceNumber       exch_seq_no_;
    UserId               user_id_;
    OrderStatus          status_;
    OrderType            order_type_ = OrderType::Limit;
    Order               *next_       = nullptr;
    Order               *prev_       = nullptr;
};
} // namespace exch_ns