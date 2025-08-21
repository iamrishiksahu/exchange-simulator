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
    FOK,
    IOC,
    Limit,
    Market,
    StopLimit,
    StopMarket,
    MarketToLimit
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

enum class ErrorCode
{
    NoError,
    CouldNotFindOrder,
    OutOfMemory
};
} // namespace exch_ns