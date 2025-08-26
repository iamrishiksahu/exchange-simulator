#pragma once
#include "instrument.h"
#include "product.h"
#include "product_instrument_structs.h"
#include <unordered_map>

namespace exch_ns
{
class ProductDataManager
{
  public:
    static ProductDataManager &GetInstance()
    {
        static ProductDataManager instance;
        return instance;
    }

    bool Init()
    {
        /* Download all instrument and product definitions */
        return true;
    }

    Instrument *GetInstrumentById(const InstrumentId id) const
    {
        auto it = instrument_id_vs_instrument_.find(id);
        if (it != instrument_id_vs_instrument_.end())
        {
            return it->second;
        }
        return nullptr;
    }

    Product *GetProductById(const ProductId id) const
    {
        auto it = product_id_vs_product_.find(id);
        if (it != product_id_vs_product_.end())
        {
            return it->second;
        }
        return nullptr;
    }

  private:
    std::unordered_map<InstrumentId, Instrument *> instrument_id_vs_instrument_;
    std::unordered_map<ProductId, Product *>       product_id_vs_product_;

  private:
    ProductDataManager()                                      = default;
    ~ProductDataManager()                                     = default;
    ProductDataManager(const ProductDataManager &)            = delete;
    ProductDataManager &operator=(const ProductDataManager &) = delete;
    ProductDataManager(ProductDataManager &&)                 = delete;
    ProductDataManager &operator=(ProductDataManager &&)      = delete;
};
}; // namespace exch_ns