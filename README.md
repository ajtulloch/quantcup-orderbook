QuantCup limit order book in C++
================================

## Matching Engine in C++ ##

This is an implementation of the limit order matching engine interface
from the [QuantCup][] problem.

The winning implementation was a classic C implementation - hand-coded
intrusive linked lists, global variables, etc.

I wanted to see if it was possible to reimplement this in C++ with
heavy usage of C++ STL and Boost libraries to reduce the amount of
code used, to reduce the amount of code, not hand-roll our own
data structures, etc.  It seemed an interesting question to see if such
a solution could be as fast as the highly-tuned C implementation.

## Files ##

Relevant files are

+ `order_book.cpp`
+ `order_book.h`
+ `constants.h`
+ `types.h`

The rest is provided by the QuantCup competition.

## Performance ##

On my 2.4 GHz Intel i7, there is no statistically signficant
difference between the winning C implementation and the C++
implementation.  The C++ implementation is significantly cleaner
(eschewing global variables, using `boost::intrusive` for our linked
list implementation, etc).

## Replication ##

To replicate these results, just run:

    $ git checkout winning-cpp-engine
    $ make build && build/test && for i in {1..10}; do echo $i && \
    build/score 2>&1 | tail -n1 | cut -d ' ' -f 3; done
    
    $ git checkout winning-c-engine
    $ make build && build/test && for i in {1..10}; do echo $i && \
    build/score 2>&1 | tail -n1 | cut -d ' ' -f 3; done

[QuantCup]: http://www.quantcup.org/home/spec
