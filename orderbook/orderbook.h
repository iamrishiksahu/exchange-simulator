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
  public:
    Order(Quantity quantity, Price price, Side side, OrderId parent_id, Timestamp client_sent_time, SequenceNumber seq_no, UserId user_id)
        : quantity_{quantity}, price_{price}, side_{side}, parent_id_{parent_id_}, client_sent_time_{client_sent_time},
          exch_seq_no_{seq_no}, user_id_{user_id}
    {
    }

    inline const std::optional<Price> GetLimitPrice() const
    {
        return price_;
    }

    inline const Quantity GetQuantity() const
    {
        return quantity_;
    }

    inline const OrderId GetOrderId() const
    {
        return id_;
    }

    inline const Side GetSide() const
    {
        return side_;
    }

  private:
    OrderId              id_;
    Quantity             quantity_;
    std::optional<Price> price_;
    Side                 side_;
    OrderId              parent_id_;
    Timestamp            exch_recv_time_;
    Timestamp            client_sent_time_;
    SequenceNumber       exch_seq_no_;
    UserId               user_id_;
    OrderStatus          status_;
};

class OrderQueue
{
    struct OrderItem
    {
        OrderId    id;
        Quantity   quantity;
        OrderItem *prev = nullptr;
        OrderItem *next = nullptr;
    };

  public:
    inline ErrorCode Enqueue(const Order &order)
    {
        // todo: perform the allocation through a pool allocator.
        OrderItem item{order.GetOrderId(), order.GetQuantity(), nullptr, nullptr};
        last_->next                         = &item;
        order_location_[order.GetOrderId()] = &item;

        return ErrorCode::NoError;
        // if allocation fails, return false;
    }

    inline ErrorCode Dequeue(const Order &order)
    {
        auto itr = order_location_.find(order.GetOrderId());
        if (itr != order_location_.end())
        {
            auto order_item  = itr->second;
            order_item->prev = order_item->next;
            return ErrorCode::NoError;
        }
        return ErrorCode::CouldNotFindOrder;
    }

  private:
    std::unordered_map<OrderId, OrderItem *> order_location_;
    OrderItem                               *first_ = nullptr;
    OrderItem                               *last_  = nullptr;
};

class PriceLevel
{
  public:
    PriceLevel()
    {
    }

    inline void Add(const Order &order)
    {
        total_quantity_ += order.GetQuantity();
    }

    inline Quantity GetTotalQuantity() const
    {
        return total_quantity_;
    }

  private:
    OrderQueue order_queue_;
    Quantity   total_quantity_ = Quantity(0);
};

template <typename Comparator> class Book
{
  public:
    explicit Book(Side side) : side_{side}
    {
    }

    inline void AddOrder(const Order &order)
    {
        price_levels_[order.GetLimitPrice().value()].Add(order);
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
    Side                                    side_;
    std::map<Price, PriceLevel, Comparator> price_levels_;
    uint32_t                                total_order_count_     = 0;
    Quantity                                total_order_quanitity_ = Quantity(0);
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