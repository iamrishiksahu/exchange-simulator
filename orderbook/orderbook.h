#pragma once
#include "../common/order_enums.h"
#include "book.h"
#include "order.h"
#include "price_level.h"
#include <iostream>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

namespace exch_ns
{
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

    inline void AddOrder(Order &order)
    {
        order.GetSide() == Side::Buy ? buy_book_.AddOrder(order) : sell_book_.AddOrder(order);
    }

  private:
    Book<std::greater<Price>> buy_book_{Side::Buy};
    Book<std::less<Price>>    sell_book_{Side::Sell};
};
} // namespace exch_ns