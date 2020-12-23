<h1 align=center> VLSI testing - Assignment 6 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

[TOC]

## Part.a

### Run

```
./atpg -bt [backtrack_number] -output [pattern][circuit]

# Example
./atpg -bt 1 -output b17_1_vectors b17.bench
```

### Results

| -bt  | \# patterns | fault coverage (%) | CPU run time (s) | actual \# backtrack |
| :--: | :---------: | :----------------: | :--------------: | :-----------------: |
|  1   |    41647    |       55.00        |      838.08      |        68413        |
|  10  |    72511    |       82.09        |     1235.09      |       376982        |
| 100  |    83711    |       90.01        |     1520.50      |       1821691       |
| 1000 |    86025    |       91.62        |     3767.59      |      12664818       |

### Discussion

When the number of backtrack increases, fault coverage increases but running time also becomes longer. 





## Part.b

### Run

```
./atpg -input [pattern] [circuit]

# Example
./atpg -input b17_1_vectors b17.bench 
```





## Part.c

### net17 stuck-at-0





### n60 stuck-at-1





## Part.d





## Part.e

