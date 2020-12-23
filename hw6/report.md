<h1 align=center> VLSI testing - Assignment 6 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

[TOC]

## Part.a

### Run

```
./atpg -bt <backtrack_number> -output <output_file> <circuit>

# Example
./atpg -bt 1 -output b17_1_vectors b17.bench
```

### Result

| -bt  | \# patterns | fault coverage (%) | CPU run time (s) | actual \# backtrack |
| :--: | :---------: | :----------------: | :--------------: | :-----------------: |
|  1   |    41647    |       55.00        |      838.08      |        68413        |
|  10  |    72511    |       82.09        |     1235.09      |       376982        |
| 100  |    83711    |       90.01        |     1520.50      |       1821691       |
| 1000 |             |                    |                  |                     |

### Discussion

When the number of backtrack increases, fault coverage increases but running time also becomes longer. 





## Part.b





## Part.c





## Part.d





## Part.e

