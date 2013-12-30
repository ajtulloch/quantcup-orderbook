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



[QuantCup]: http://www.quantcup.org/home/spec


