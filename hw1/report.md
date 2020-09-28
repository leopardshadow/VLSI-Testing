<h1 align=center> VLSI testing - Assignment 1 </h1>

[TOC]

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

## Overwiew

In this probelm, the whole circuit can be treated as a directed graph.





## Algorithm

### DFS search

An straightforward approach would be simply using DFS search.
The algorithm is described as follows.

```
dfs (g, path)
    if g is not end_gate
        for every h in g's fanout 
            dfs(h, path + g)
    if g is end_gate
        print (path + g)
        
dfs(start_gate, "")
```

However, the whole process can be very long.

To shorten the time, I applied following strategies below.



### Remove non-active gate

Consider the case below. The only path from PI to PO is the upper one. However, when doing DFS search, the path PI --> G1 --> O1 is also considered. It will not know it's an unvalid path until they reach O1, which has no fanout.

When the circuit size become bigger, the differcnes will affect the execution time significantly. 

```
I1 --┐
     ┢━━━━━━━━━━━PO
  ┏━━┛
PI
  └--┐
     |--- G1 --- O1
I2 --┘
```






## Test Cases Result








## Build

```
make
./atpg -path -start <PI> -end <PO> <circuit_name>
```
