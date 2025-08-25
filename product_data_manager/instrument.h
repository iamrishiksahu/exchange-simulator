#include "../common/market_state.h"
#include "product_instrument_structs.h"
#include <string>

namespace exch_ns
{
class Instrument
{
  public:
    const InstrumentId GetId() const noexcept
    {
        return id_;
    }

    const std::string &GetName() const noexcept
    {
        return name_;
    }

    const std::string &GetDescription() const noexcept
    {
        return description_;
    }

    inline const double GetPriceDivisor() const noexcept
    {
        return price_divisor_;
    }

    inline const TickSize GetTickSize() const noexcept
    {
        return tick_size_;
    }

    const std::string &GetSymbol() const noexcept
    {
        return symbol_;
    }

    const InstrumentType GetType() const noexcept
    {
        return type_;
    }

    void SetInstrumentStatus(const InstrumentStatus status) noexcept
    {
        status_ = status;
    }

    const std::string &GetProductName() const noexcept
    {
        return product_name_;
    }

    const uint16_t GetMinimumQuantity() const noexcept
    {
        return minimum_quantity_;
    }

  private:
    InstrumentId     id_;
    std::string      name_;
    std::string      symbol_;
    std::string      description_;
    std::string      product_name_;
    int16_t          price_divisor_    = 1;
    TickSize         tick_size_        = 1;
    uint16_t         minimum_quantity_ = 1;
    uint16_t         market_state_     = MarketState::UNKNOWN;
    InstrumentType   type_             = InstrumentType::FUTURE;
    InstrumentStatus status_           = InstrumentStatus::ACTIVE;
};
}; // namespace exch_ns