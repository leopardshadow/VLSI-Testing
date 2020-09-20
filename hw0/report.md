<h1 align=center> VLSI testing - Assignment 0 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

Objective of this homework is to get familiar with PODEM code.

The whole circuit is stored in the variable `Circuit`, which is an extern `CIRCUIT`-type variable.

The type `CIRCUIT` defines how the circuit is composed.
In this assignment, input list, output list and gate list are used.

The gate list stores the information of gates.
Information of a gate used in this assignment includes gate function and number of fanouts.

---

In the beginning, I get confused with signal nets, branch nets and stem nets.

For a single ouput gate, there is one signal net, but neither stem nor branch.
Stem nets and branch nets are defined only if there are at least two fanouts for a gate.

Hence, the relation between them should be 
``` 
number of signal nets = number of stem nets + number of branch nets + number of gates with only one fanout
```

