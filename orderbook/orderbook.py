from collections import deque
from typing import List, Dict 
from sortedcontainers import SortedDict
from dataclasses import dataclass, field
import itertools
from decimal import Decimal
from enum import Enum

_order_counter = itertools.count(1)
_trade_counter = itertools.count(1)

class Side(Enum):
    BUY = 1
    SELL = 2

@dataclass
class Order:
    side: Side = Side.BUY
    qty: int = 0
    price: Decimal = 0
    instrument_id: int = 0
    filled_qty: int = 0
    id: int = field(default_factory=lambda: next(_order_counter))
    
    @property
    def remaining(self) -> int:
        return self.qty - self.filled_qty
    
    def __repr__(self):
        return f"Order: {self.side} {self.price} {self.qty}"

    
@dataclass
class Trade:
    trade_qty: int
    trade_price: Decimal
    side: Side
    aggressor_order_id: int
    resting_order_id: int
    id: int = field(default_factory=lambda: next(_order_counter))
    
    def __repr__(self):
        return f"Trade: {self.trade_qty}, {self.trade_price}, {self.side}"

class Orderbook():
    def __init__(self):
        self.bids: SortedDict[Decimal, deque[Order]] = SortedDict()
        self.asks: SortedDict[Decimal, deque[Order]] = SortedDict()
        self.trades: List[Trade] = []
        self.order_map: Dict[int, Order] = {}
        
    def __best_ask(self) -> Decimal | None:
        return self.asks.peekitem(0)[0] if self.asks else None
    
    def __best_bid(self):
        return self.bids.peekitem(-1)[0] if self.bids else None
    
    def add_order(self, side, price, qty):
        order = Order(side, qty, price, 1)
        print(order)
        self.__match(order)
        
        if order.remaining > 0:
            book = self.bids if side == Side.BUY else self.asks
            if price not in book:
                book[price] = deque()
            book[price].append(order)
            self.order_map[order.id] = order
                
    def __process_trade(self, aggressor: Order, resting: Order, match_price: Decimal, match_qty: int):
        
        aggressor.filled_qty += match_qty
        resting.filled_qty += match_qty
        
        trade_obj = Trade(match_qty, match_price, aggressor.side, aggressor.id, resting.id)
        print(trade_obj)
        self.trades.append(trade_obj)
        
        if resting.remaining == 0:
            self.order_map.pop(resting.id)
            if resting.side == Side.BUY:
                q: deque[Order] = self.bids[resting.price]
                q.popleft()
                
                if not q:
                    self.bids.pop(resting.price)
            else:
                q: deque[Order] = self.asks[resting.price]
                q.popleft()

                if not q:
                    self.asks.pop(resting.price)
        
    
    def __match(self, aggressor: Order):
        if aggressor.side == Side.BUY:
            while aggressor.remaining > 0 and self.asks:
                best_ask_price = self.__best_ask()
                if best_ask_price > aggressor.price: break
                resting:Order = self.asks[best_ask_price][0]
                match_qty = min(aggressor.remaining, resting.remaining)
                self.__process_trade(aggressor, resting, best_ask_price, match_qty)
        else:
            while aggressor.remaining > 0 and self.bids:
                best_bid_price = self.__best_bid()
                if best_bid_price < aggressor.price: break
                resting: Order = self.bids[best_bid_price][0]
                match_qty = min(aggressor.remaining, resting.remaining)
                self.__process_trade(aggressor, resting, best_bid_price, match_qty)


def main():
    ob = Orderbook()
    
    ob.add_order(Side.SELL, Decimal("103.0"), 70)
    ob.add_order(Side.SELL, Decimal("102.0"), 100)
    ob.add_order(Side.SELL, Decimal("101.0"), 50)

    # Add some BUY orders (aggressor)
    ob.add_order(Side.BUY, Decimal("102.0"), 80)  # Should match 50@101 + 30@102
    ob.add_order(Side.BUY, Decimal("103.0"), 50)  # Should match remaining 70@102 partially

    print("Bids (Buy side):")
    for price, orders in ob.bids.items():
        print(f"{price}: {[order.remaining for order in orders]}")

    print("\nAsks (Sell side):")
    for price, orders in ob.asks.items():
        print(f"{price}: {[order.remaining for order in orders]}")


if __name__ == "__main__":
    main()