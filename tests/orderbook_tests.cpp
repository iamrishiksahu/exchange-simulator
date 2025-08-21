#include "../orderbook/orderbook.h"
#include "test_utils/test_order_factory.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

namespace exch_ns
{
class OrderbookTests : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
        for (Order *order : test_wise_orders_)
        {
            delete order;
        }
    }

    static void SetUpTestSuite()
    {
    }
    static void TearDownTestSuite()
    {
    }

    Order *GetNewOrder(Price price, Quantity qty, Side side = Side::Sell)
    {
        Order *ord = TestOrderFactory::Create(price, qty, side, ++seq_no_);
        test_wise_orders_.emplace_back(ord);
        return ord;
    }

  private:
    SequenceNumber       seq_no_ = 0;
    std::vector<Order *> test_wise_orders_;
};

TEST_F(OrderbookTests, SimpleOrderbookGetBestBidAndAskPrice)
{
    Orderbook ob;

    EXPECT_EQ(ob.GetBestAskPrice(), std::nullopt);
    EXPECT_EQ(ob.GetBestBidPrice(), std::nullopt);

    ob.AddOrder(*(GetNewOrder(4, 10)));
    ob.AddOrder(*(GetNewOrder(2, 10)));
    ob.AddOrder(*(GetNewOrder(5, 10)));

    EXPECT_EQ(ob.GetBestAskPrice(), Price{2});
    EXPECT_EQ(ob.GetBestBidPrice(), std::nullopt);

    ob.AddOrder(*(GetNewOrder(0, 10, Side::Buy)));
    ob.AddOrder(*(GetNewOrder(-1, 10, Side::Buy)));
    ob.AddOrder(*(GetNewOrder(-2, 10, Side::Buy)));

    EXPECT_EQ(ob.GetBestAskPrice(), Price{2});
    EXPECT_EQ(ob.GetBestBidPrice(), Price{0});
}

} // namespace exch_ns