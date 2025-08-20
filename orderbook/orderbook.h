#pragma once
#include "../common/order_enums.h"
#include "types.h"
#include <iostream>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

namespace exch_ns
{

class OrderEvent
{
};

class Order
{
    /**
     * An instance of this class can only be created by
     * the MatchingEngine orchestrator, and by the OrderFactoryForTest
     * in unit testing build.
     */
    friend class MatchingEngine;
#ifdef UNIT_TESTING
    firend class OrderFactoryForTest;
#endif

  public:
    inline const std::optional<Price> GetLimitPrice() const noexcept
    {
        return price_;
    }

    inline const std::optional<Price> GetStopTriggerPrice() const noexcept
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
    OrderType            order_type_;
    Order               *next_ = nullptr;
    Order               *prev_ = nullptr;
};

class FifoOrderQueue
{
  public:
    inline ErrorCode Enqueue(Order *order)
    {
        if (last_ == nullptr)
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

template <typename Comparator> class Book
{
  public:
    explicit Book(Side side) : side_{side}
    {
        side == Side::Sell ? price_direction_multiplier_ = -1 : price_direction_multiplier_ = 1;
    }

    inline void AddOrder(Order &order)
    {
        switch (order.GetType())
        {
        case OrderType::Limit:
            price_levels_[order.GetLimitPrice()].Add(order);
            break;
        case OrderType::StopLimit:
        case OrderType::StopMarket:
            price_levels_[order.GetStopTriggerPrice()].AddStop(order);
            break;
        case OrderType::MarketToLimit:
            if (__builtin_expect(price_levels_.empty(), 0)) /* "0" = unlikely */
                return std::nullopt;
            Price new_price = GetBestPrice() + market_to_limit_protection_ * price_direction_multiplier_;
            AddOrder(new_price, order);
            break;
        case OrderType::Market:
        case OrderType::IOC:
            /** We do not allow market orders to get stored in the book
             * We have IOC policy for market orders
             */
            break;
        case OrderType::FOK:
            /* If the order can be fully filled, then only we try to fill else cancel */
            break;
        default:
            /* Log: Unhandled order type */
            break;
        }
        total_order_quanitity_ += order.GetQuantity();
    }

    inline const Side GetSide() const
    {
        return side_;
    }

    inline std::optional<Price> GetBestPrice() const noexcept
    {
        if (__builtin_expect(price_levels_.empty(), 0)) /* "0" = unlikely */
            return std::nullopt;
        return price_levels_.begin()->first;
    }

    inline uint32_t GetTotalOrderCount() const
    {
        return total_order_count_;
    }

    inline uint32_t GetTotalOrderQuantity() const
    {
        return total_order_quanitity_;
    }

  private:
    inline void AddOrder(Price price, Order &order)
    {
        price_levels_[price].Add(order);
    }

  private:
    Side                                    side_;
    std::map<Price, PriceLevel, Comparator> price_levels_;
    uint32_t                                total_order_count_          = 0;
    Quantity                                total_order_quanitity_      = Quantity(0);
    Price                                   market_to_limit_protection_ = 10; /* Should be determined by nature of contract */
    int                                     price_direction_multiplier_ = 1;
};

class Orderbook
{
  public:
    Orderbook() = default;

    inline const Book<std::greater<Price>> &GetBuyBook() const
    {
        return buy_book_;
    }

    inline const Book<std::less<Price>> &GetSellBook() const
    {
        return sell_book_;
    }

    inline const std::optional<Price> GetBestBidPrice() const
    {
        return buy_book_.GetBestPrice();
    }

    inline const std::optional<Price> GetBestAskPrice() const
    {
        return sell_book_.GetBestPrice();
    }

    inline void AddOrder(Order order)
    {
        order.GetSide() == Side::Buy ? buy_book_.AddOrder(order) : sell_book_.AddOrder(order);
    }

  private:
    Book<std::greater<Price>> buy_book_{Side::Buy};
    Book<std::less<Price>>    sell_book_{Side::Sell};
};
} // namespace exch_ns