#include "fifo_order_queue.h"
#include "order.h"
#include "price_level.h"
#include <map>

namespace exch_ns
{
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
            price_levels_[order.GetLimitPrice().value()].Add(order);
            break;
        case OrderType::StopLimit:
        case OrderType::StopMarket:
            price_levels_[order.GetStopTriggerPrice().value()].AddStop(order);
            break;
        case OrderType::MarketToLimit: {
            if (__builtin_expect(price_levels_.empty(), 0)) /* "0" = unlikely */
                return;
            Price new_price = GetBestPrice().value() + (double) market_to_limit_protection_ * (double) price_direction_multiplier_;
            this->AddOrder(new_price, order);
            break;
        }
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

} // namespace exch_ns