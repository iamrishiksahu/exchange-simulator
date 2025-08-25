#pragma once
#include "../orderbook/types.h"
#include "order_enums.h"
#include <variant>

namespace exch_ns
{
struct MinimumOrderParameters
{
    Side         side;
    UserId       user_id;
    OrderId      order_id;
    InstrumentId instrument_id;
    Timestamp    exch_recv_time;
    Timestamp    client_sent_time;
};

struct NewOrder : MinimumOrderParameters
{
    Quantity       quantity;
    Price          price;
    Price          stop_trigger_price;
    OrderId        parent_id;
    OrderStatus    status;
    OrderType      order_type;
    SequenceNumber exch_seq_no;
};

struct CancelOrder : MinimumOrderParameters
{
    Price price;
};

struct PriceModification
{
    Price new_price;
};

struct QuantityModification
{
    Quantity new_quantity;
};

using Modification = std::variant<PriceModification, QuantityModification>;

struct ModifyOrder : MinimumOrderParameters
{
    Modification modification;
};
}; // namespace exch_ns