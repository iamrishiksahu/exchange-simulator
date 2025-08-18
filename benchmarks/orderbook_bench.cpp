#include "../orderbook/orderbook.h"
#include <benchmark/benchmark.h>

static void BM_NestedCallChain(benchmark::State &state)
{
    exch_ns::Orderbook ob;
    for (auto _ : state)
    {
        using namespace exch_ns;

        ob.AddOrder(Order(Quantity{1}, Price{1}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{2}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{3}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{4}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{5}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{5}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{5}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{5}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{6}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{6}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{6}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        ob.AddOrder(Order(Quantity{1}, Price{6}, Side::Sell, OrderId{123}, Timestamp{12312}, 1, UserId{1}));
        
        ob.GetBestAskPrice();
        ob.GetBestBidPrice();
    }
}
BENCHMARK(BM_NestedCallChain);

BENCHMARK_MAIN();