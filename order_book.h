#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/intrusive/slist.hpp>
#include <boost/intrusive/list.hpp>

#include "types.h"

namespace OB {

class OrderBook : public boost::noncopyable {
 public:
  static OrderBook& get();

  void initialize();

  void shutdown();

  // TODO: take by value?
  t_orderid limit(t_order& order);

  void cancel(t_orderid orderid);

 private:
  OrderBook() {}

  struct OrderBookEntry : public boost::intrusive::slist_base_hook<> {
    t_size size{0}; /* Order size * */
    Field trader{};
  };

  // pricePoint: describes a single price point in the limit order book.
  typedef boost::intrusive::slist<
      OrderBookEntry, boost::intrusive::cache_last<true> > PricePoint;

  // An array of pricePoint structures representing the entire limit order book
  std::vector<OrderBookEntry> arenaBookEntries;
  std::vector<PricePoint> pricePoints;

  // Monotonically-increasing orderID
  t_orderid curOrderID;
  // Minimum Ask price
  t_price askMin;
  // Maximum Bid price
  t_price bidMax;
};
}
