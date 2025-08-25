#include "product_instrument_types.h"
#include <string>

namespace exch_ns
{
struct InstrumentData
{
    InstrumentId id;
    std::string  name;
    ProductId    product_id;
};

enum class ProductType
{
    FUTURE,
    OPTION,
    SPOT
};

enum class InstrumentType
{
    FUTURE,
    OPTION,
    SPOT,
    INDEX
};

enum class InstrumentStatus
{
    ACTIVE,
    EXPIRED
};

}; // namespace exch_ns