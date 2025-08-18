#include "../orderbook/orderbook.h"
#include <gtest/gtest.h>

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
    }

    static void SetUpTestSuite()
    {
    }
    static void TearDownTestSuite()
    {
    }

    Order GetNewOrder(Price price, Quantity qty, Side side = Side::Sell)
    {
        ++seq_no_;
        return Order(qty, price, side, OrderId{123}, Timestamp{12312}, seq_no_, UserId{1});
    }

  private:
    SequenceNumber seq_no_ = 0;
};

TEST_F(OrderbookTests, SimpleOrderbookGetBestBidAndAskPrice)
{
    Orderbook ob;

    EXPECT_EQ(ob.GetBestAskPrice(), std::nullopt);
    EXPECT_EQ(ob.GetBestBidPrice(), std::nullopt);

    ob.AddOrder(GetNewOrder(4, 10));
    ob.AddOrder(GetNewOrder(2, 10));
    ob.AddOrder(GetNewOrder(5, 10));

    EXPECT_EQ(ob.GetBestAskPrice(), Price{2});
    EXPECT_EQ(ob.GetBestBidPrice(), std::nullopt);

    ob.AddOrder(GetNewOrder(0, 10, Side::Buy));
    ob.AddOrder(GetNewOrder(1, 10, Side::Buy));
    ob.AddOrder(GetNewOrder(-2, 10, Side::Buy));

    EXPECT_EQ(ob.GetBestAskPrice(), Price{2});
    EXPECT_EQ(ob.GetBestBidPrice(), Price{1});
}

} // namespace exch_ns