#include "../common/double_helper.h"
#include "../product_data_manager/instrument.h"
#include <cmath>
#include <cstdint>

namespace exch_ns
{
using Quantity       = uint32_t;
using OrderId        = uint64_t;
using Timestamp      = uint64_t; // nanos since epoch
using SequenceNumber = uint64_t;
using UserId         = uint64_t;
using InstrumentId   = uint64_t;

class Price
{
  public:
    Price(double price, const Instrument *inst) : value_(price), instrument_(inst)
    {
    }

    static Price FromDouble(double price, const Instrument *inst)
    {
        return Price{static_cast<int64_t>(std::llround(price * inst->GetPriceDivisor())), inst};
    }

    static Price FromString(const char *price_str, const Instrument *inst)
    {
        char  *endptr = nullptr;
        double val    = std::strtod(price_str, &endptr);

        return FromDouble(val, inst);
    }

    const double ToDouble() const noexcept
    {
        double value = value_ / instrument_->GetPriceDivisor();
        DoubleHelper::RoundToTick(value, instrument_->GetTickSize());
        return value;
    }

    const TickSize GetTickSize() const noexcept
    {
        return instrument_->GetTickSize();
    }

  private:
    int64_t           value_;
    const Instrument *instrument_;
};
} // namespace exch_ns