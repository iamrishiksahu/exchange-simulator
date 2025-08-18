#include <cstdint>

namespace exch_ns
{
using Price          = double;
using Quantity       = uint32_t;
using OrderId        = uint64_t;
using Timestamp      = uint64_t; // nanos since epoch
using SequenceNumber = uint64_t;
using UserId         = uint64_t;
} // namespace exch_ns