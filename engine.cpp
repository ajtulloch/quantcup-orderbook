/*****************************************************************************
 *                QuantCup 1:   Price-Time Matching Engine
 *
 * Submitted by: voyager
 *
 * Design Overview:
 *   In this implementation, the limit order book is represented using
 *   a flat linear array (pricePoints), indexed by the numeric price value.
 *   Each entry in this array corresponds to a specific price point and holds
 *   an instance of struct pricePoint. This data structure maintains a list
 *   of outstanding buy/sell orders at the respective price. Each outstanding
 *   limit order is represented by an instance of struct orderBookEntry.
 *
 *   askMin and bidMax are global variables that maintain starting points,
 *   at which the matching algorithm initiates its search.
 *   askMin holds the lowest price that contains at least one outstanding
 *   sell order. Analogously, bidMax represents the maximum price point that
 *   contains at least one outstanding buy order.
 *
 *   When a Buy order arrives, we search the book for outstanding Sell orders
 *   that cross with the incoming order. We start the search at askMin and
 *   proceed upwards, incrementing askMin until:
 *     a) The incoming Buy order is filled.
 *     b) We reach a price point that no longer crosses with the incoming
 *        limit price (askMin > BuyOrder.price)
 *     In case b), we create a new orderBookEntry to record the
 *     remainder of the incoming Buy order and add it to the global order
 *     book by appending it to the list at pricePoints[BuyOrder.price].
 *
 *  Incoming Sell orders are handled analogously, except that we start at
 *  bidMax and proceed downwards.
 *
 *  Although this matching algorithm runs in linear time and may, in
 *  degenerate cases, require scanning a large number of array slots,
 *  it appears to work reasonably well in practice, at least on the
 *  simulated data feed (score_feed.h). The vast majority of incoming
 *  limit orders can be handled by examining no more than two distinct
 *  price points and no order requires examining more than five price points.
 *
 *  To avoid incurring the costs of dynamic heap-based memory allocation,
 *  this implementation maintains the full set of orderBookEntry instances
 *  in a statically-allocated contiguous memory arena (arenaBookEntries).
 *  Allocating a new entry is simply a matter of bumping up the orderID
 *  counter (curOrderID) and returning a pointer to
 *arenaBookEntries[curOrderID].
 *
 *  To cancel an order, we simply set its size to zero. Notably, we avoid
 *  unhooking its orderBookEntry from the list of active orders in order to
 *  avoid incurring the costs of pointer manipulation and conditional branches.
 *  This allows us to handle order cancellation requests very efficiently; the
 *  current implementation requires only one memory store instruction on
 *  x86_64. During order matching, when we walk the list of outstanding orders,
 *  we simply skip these zero-sized entries.
 *
 *  The current implementation uses a custom version of strcpy() to copy the
 *string
 *  fields ("symbol" and "trader") between data structures. This custom version
 *  has been optimized for the case STRINGLEN=5 and implements loop unrolling
 *  to eliminate the use of induction variables and conditional branching.
 *
 *  The memory layout of struct orderBookEntry has been optimized for
 *  efficient cache access.
 *****************************************************************************/

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "engine.h"
#include "order_book.h"
#include <boost/intrusive/slist.hpp>
#include <boost/intrusive/list.hpp>
#include "util.h"

void init() { OB::OrderBook::get().initialize(); }

void destroy() { OB::OrderBook::get().shutdown(); }

/* Process an incoming limit order */
t_orderid limit(t_order order) { return OB::OrderBook::get().limit(order); }

/* Cancel an outstanding order */
void cancel(t_orderid orderid) { OB::OrderBook::get().cancel(orderid); }
