# VLSI-Testing

my solutions to VLSI testing course

[Course Website \(2020Fall\)](http://tiger.ee.nctu.edu.tw/course/Testing2020Fall/schedule.html)

My reviews on this course can be found in [my blog](https://blog.cchenglo.com/posts/nctu/vlsi-testing/).

* hw0 - basic understanding to the provided PODEM code
* hw1 - find all the paths from designated pair of input and output
* hw2
  * logic simulation
  * random test pattern generation
  * logic simulation with unknown encoded
* hw3
  * pack different input patterns into one a simulation run
  * correlations between \#gate evaluations and packed patterns
  *  generates compiled code simulation
* hw4 - 
  * checkpoint theorem
  * custom faults model (bridging faults)
* hw5 - 
  * checkpoint theorem fsim
  * pack different \#fsim per pass
  * parallel fsim for bridging faults (cont'd hw4)
* hw6 - 
  * podem w/ different backtrack limit
  * ATPG: total vs. checkpoint fault list
  * trace PODEM
  * ATPG with random pattern generation first
  * ATPG for bridging faults (cont'd hw5)

## Prerequisites

* make
* g++
* `sudo apt-get install bison flex` (lex.yy.cc, readcircuit.tab.cc, readcircuit.tab.h)
* `sudo apt-get install libreadline-dev`


