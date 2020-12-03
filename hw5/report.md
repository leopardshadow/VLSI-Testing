<h1 align=center> VLSI testing - Assignment 5 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

[TOC]

## Part.a

Take c17.bench for example. The instructions are described as follows.

### Run

1. generate patterns for a circuit by running built-in ATPG

```
./atpg -output c17.atpg ../circuits/iscas85/c17.bench
```

2. run fault simulation to obtain fault coverages

```
./atpg -fsim -input c17_atpg c17.bench 
```

3. run fault simulation with check point theorem to obtain fault coverage

```
./atpg -check_point -input c17_atpg c17.bench 
```

### Results





## Part.b

### Run

The number is defined in line 18 in the file `typeemu.h`.

```c++
const unsigned PatternNum = 16;
```



### Results

| CPU-time (s) |   1   |  4   |  8   |  16  |  32  |   64   |
| :----------: | :---: | :--: | :--: | :--: | :--: | :----: |
|  c17.bench   | 0.35  | 0.35 | 0.35 | 0.35 | 0.35 |  0.35  |
|  c499.bench  | 18.68 |      |      |      |      | 17.08  |
| c7552.bench  |       |      |      |      |      | 204.42 |

### Discussion

The case c17.bench is too small to see the changes.

The second case, c499.bench, in the contrary, are affected greatly by the number of faults per pass.

 

## Part.c

### Algorithm

The fault simulation for bridging faults are described below.

```
for different patterns {
	run fault-free simulation
	single pattern parallel bridging fault simulation (
		set fault-free value for every gates
		for all undetected faults f (@gate g) {
			if f redundant: skip
			if f not activated: skip
			if f can be seen directly: skip
			if g is stem:
			
			if g is branch:
				
		}
		do fault simulation
    )
}
```

