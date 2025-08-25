#pragma once
namespace exch_ns
{
enum class Side
{
    Buy,
    Sell,
    Unknown
};

enum class OrderType
{
    Limit,
    Market,
    StopLimit,
    StopMarket,
    MarketToLimit
};

enum class OrderTif
{
    Day, // Remains working till the current trading session end
    Gtc, // Remains working until cancelled
    Gtd, // Remains working until given date's trading session end
    Fok, // Either filled fully or cancelled immediately
    Ioc  // Tried matching, if quantity is left, cancelled immediately.
};

enum class ManulaOrderIndicator
{
    Manual,
    Automatic
};

enum class OrderEventType
{
    New,
    Trade,
    Cancelled,
    Rejected
};

enum class OrderStatus
{
    New,
    PartFilled,
    Filled,
    Cancelled,
    Rejected
};

enum class ModificationType
{
    Price,
    Quantity
};

enum class ErrorCode
{
    NoError,
    CouldNotFindOrder,
    OutOfMemory
};
} // namespace exch_ns