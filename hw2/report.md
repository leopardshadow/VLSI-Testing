<h1 align=center> VLSI testing - Assignment 2 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

[TOC]

## Overview





## Part.1 

The command for logic simulation on a circuit given a pattern is

```
./atpg -logicsim -input <input_pattern> <circuit>
```

### simulation of circuit c17.bench given test pattern c17.input

```
./atpg -logicsim -input c17.input ../../circuits/iscas85/c17.bench
```

```
Start parsing input file
Finish reading circuit file
Run logic simulation
01110 00
10101 11
00101 01
01000 11
10001 01
00011 01
00111 00
00000 00
total CPU time = 0.000114
```

## Part.2-a





### Random Number Generation



### Memory Usage

There are several ways to show the memory usage of a program.

* GNU time



### gnu-time on mac

```
brew install gnu-time
gtime ...
```





generates different number of simulations given a circuit and print the result to designated file

```
./atpg -pattern -num <number_of_sim.> -output <output_file> <circuit_file>
```

```
bash run2a.bash | grep 'time'
```





## Part.2-b

A straightforward approach to encode `0`, `1` and `X` would be represent `0` as 00, `1` as 11 and `X` as 01. Using this encoding, and/or operations works fine !

However, when we invert `X`, 01 becomes 10. Inverted-`X` should also be unknown, so we represent `X` as 01 or 10 instead.

But new problems pop out ! X AND inverted-X should be still X since the two Xs might come from different places with different actual values. Hence, some modification need to be done to solve this.

The only problem comes from invert. So I revise the original invert function, which shown as follows.

|  a   | representation of a |  a'  | representation of a' |
| :--: | :-----------------: | :--: | :------------------: |
|  0   |         00          |  1   |          11          |
|  1   |         11          |  0   |          00          |
|  X   |         01          |  X   |        **01**        |
|  X   |         10          |  X   |          01          |

In the begining, all unknows are encoded as 0.

By doing so, and, or, nand, nor works fine without further modification !







## Build

```
make
./atpg ... (depends on different usage)
```







```
./atpg -mod_logicsim -input c17.input c17.bench

```

