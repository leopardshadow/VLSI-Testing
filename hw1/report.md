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




## Test Cases Result








## Build

```
make
./atpg -path -start <PI> -end <PO> <circuit_name>
```
