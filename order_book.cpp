#include <string.h>

#include "engine.h"
#include "limits.h"
#include "order_book.h"
#include "constants.h"

namespace OB {

namespace {

/* Report trade execution */
void executeTrade(const Field& symbol, const Field& buyTrader,
                  const Field& sellTrader, t_price tradePrice,
                  t_size tradeSize) {
  t_execution exec;

  if (tradeSize == 0) /* Skip orders that have been cancelled */
    return;

  exec.symbol = symbol;

  exec.price = tradePrice;
  exec.size = tradeSize;

  exec.side = 0;
  exec.trader = buyTrader;
  execution(exec); /* Report the buy-side trade */

  exec.side = 1;
  exec.trader = sellTrader;
  execution(exec); /* Report the sell-side trade */
}
}

OrderBook& OrderBook::get() {
  static OrderBook ob;
  return ob;
}

void OrderBook::initialize() {
  /* Initialize the price point array */
  pricePoints.resize(kMaxPrice);
  for (auto& el : pricePoints) {
    el.clear();
  }

  arenaBookEntries.resize(kMaxNumOrders);

  curOrderID = 0;
  askMin = kMaxPrice;
  bidMax = kMinPrice;
}

void OrderBook::shutdown() {}

t_orderid OrderBook::limit(t_order& order) {
  t_price price = order.price;
  t_size orderSize = order.size;

  if (order.side == 0) {/* Buy order */
    /* Look for outstanding sell orders that cross with the incoming order */
    if (price >= askMin) {
      auto ppEntry = pricePoints.begin() + askMin;
      do {
        auto bookEntry = ppEntry->begin();
        while (bookEntry != ppEntry->end()) {
          if (bookEntry->size < orderSize) {
            executeTrade(order.symbol, order.trader, bookEntry->trader, price,
                         bookEntry->size);
            orderSize -= bookEntry->size;
            ++bookEntry;
          } else {
            executeTrade(order.symbol, order.trader, bookEntry->trader, price,
                         orderSize);
            if (bookEntry->size > orderSize)
              bookEntry->size -= orderSize;
            else
              ++bookEntry;

            ppEntry->erase(ppEntry->begin(), bookEntry);
            while (ppEntry->begin() != bookEntry) {
              ppEntry->pop_front();
            }
            return ++curOrderID;
          }
        }
        /* We have exhausted all orders at the askMin price point. Move on to
           the next price level. */
        ppEntry->clear();
        ppEntry++;
        askMin++;
      } while (price >= askMin);
    }

    auto entry = arenaBookEntries.begin() + (++curOrderID);
    entry->size = orderSize;
    entry->trader = order.trader;
    pricePoints[price].push_back(*entry);
    if (bidMax < price) bidMax = price;
    return curOrderID;

  } else {/* Sell order */
    /* Look for outstanding Buy orders that cross with the incoming order */
    if (price <= bidMax) {
      auto ppEntry = pricePoints.begin() + bidMax;
      do {
        auto bookEntry = ppEntry->begin();
        while (bookEntry != ppEntry->end()) {
          if (bookEntry->size < orderSize) {
            executeTrade(order.symbol, bookEntry->trader, order.trader, price,
                         bookEntry->size);
            orderSize -= bookEntry->size;
            ++bookEntry;
          } else {
            executeTrade(order.symbol, bookEntry->trader, order.trader, price,
                         orderSize);
            if (bookEntry->size > orderSize)
              bookEntry->size -= orderSize;
            else
              ++bookEntry;

            while (ppEntry->begin() != bookEntry) {
              ppEntry->pop_front();
            }
            return ++curOrderID;
          }
        }

        /* We have exhausted all orders at the bidMax price point. Move on to
           the next price level. */
        ppEntry->clear();
        ppEntry--;
        bidMax--;
      } while (price <= bidMax);
    }

    auto entry = arenaBookEntries.begin() + (++curOrderID);
    entry->size = orderSize;
    entry->trader = order.trader;
    pricePoints[price].push_back(*entry);
    if (askMin > price) askMin = price;
    return curOrderID;
  }
}

void OrderBook::cancel(t_orderid orderid) {
  arenaBookEntries[orderid].size = 0;
}
}
