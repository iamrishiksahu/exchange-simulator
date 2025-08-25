#include "instrument.h"
#include <unordered_map>

namespace exch_ns
{
class Product
{
  public:
    explicit Product(ProductId id, std::string name, ProductType type) : id_(id), name_(std::move(name)), type_(type)
    {
    }

    bool BelongsTo(const InstrumentId id) const noexcept
    {
        instrument_id_vs_instruments_.count(id) > 0;
    }

    void AddInstrument(const Instrument &instrument)
    {
        instrument_id_vs_instruments_.try_emplace(instrument.GetId(), instrument);
    }

    const std::unordered_map<InstrumentId, Instrument> &GetInstruments() const noexcept
    {
        return instrument_id_vs_instruments_;
    }

    size_t GetInstrumentCount() const noexcept
    {
        return instrument_id_vs_instruments_.size();
    }

    const ProductId GetProductId() const noexcept
    {
        return id_;
    }

    const std::string &GetDescription() const noexcept
    {
        return description_;
    }

    const std::string &GetProductName() const noexcept
    {
        return name_;
    }

    const ProductType GetType() const noexcept
    {
        return type_;
    }

  private:
    ProductId                                    id_;
    ProductType                                  type_;
    std::string                                  name_;
    std::string                                  description_;
    std::unordered_map<InstrumentId, Instrument> instrument_id_vs_instruments_;
};
}; // namespace exch_ns