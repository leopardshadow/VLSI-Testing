<h1 align=center> VLSI testing - Assignment 6 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

[TOC]

## Part.a

### Run

```
./atpg -bt [backtrack_number] -output [pattern] [circuit]

# Example
./atpg -bt 1 -output b17_1_vectors b17.bench
```

### Results

|   -bt    | \# patterns | fault coverage (%) | CPU run time (s) | actual \# backtrack |
| :------: | :---------: | :----------------: | :--------------: | :-----------------: |
|    1     |    41647    |       55.00        |      838.08      |        68413        |
|    10    |    72511    |       82.09        |     1235.09      |       376982        |
|   100    |    83711    |       90.01        |     1520.50      |       1821691       |
|   1000   |    86025    |       91.62        |     3767.59      |      12664818       |
| no limit |    87963    |       93.00        |     22627.57     |      95087176       |

### Discussion

When the number of backtrack increases, fault coverage increases but running time also becomes longer. 

## Part.b

### Run

```
# ATPG based on fault list using checkpoint theorem
./atpg -output -check_point [pattern] [circuit]

# ATPG based on total fault list
./atpg -output [pattern] [circuit]

# run fault simulation
./atpg -input [pattern] [circuit]
```

|            | \# patterns | fault coverage (%) - while atpg | fault coverage (%) - using fsim |
| :--------: | :---------: | :-----------------------------: | :-----------------------------: |
|  original  |    87963    |              93.00              |             97.34%              |
| checkpoint |             |                                 |                                 |



## Part.c

### net17 stuck-at-0





### n60 stuck-at-1





## Part.d

```
./atpg -random_pattern -output [output_pattern] [circuit_name]

./atpg -random_pattern -output c17pat c17.bench
```





### b17.bench

* original ATPG  

| fault coverage (%) | \# patterns | total CPU time (sec) |
| :----------------: | :---------: | :------------------: |
|                    |             |                      |

* random pattern generation + original ATPG 

| fault coverage (%) | \# patterns  | total CPU time (sec) |
| :----------------: | :----------: | :------------------: |
|   47.52 -> 93.72   | 1000 + 40156 |       9389.91        |

### s35932_com.bench

* original ATPG  

| fault coverage (%) | \# patterns | total CPU time (sec) |
| :----------------: | :---------: | :------------------: |
|                    |             |                      |

* random pattern generation + original ATPG 

| fault coverage (%) | \# patterns | total CPU time (sec) |
| :----------------: | :---------: | :------------------: |
|   89.69 -> 89.69   |  1000 + 0   |        50.60         |

### s38417_com.bench

* original ATPG  

| fault coverage (%) | \# patterns | total CPU time (sec) |
| :----------------: | :---------: | :------------------: |
|                    |             |                      |

* random pattern generation + original ATPG 

| fault coverage (%) | \# patterns | total CPU time (sec) |
| :----------------: | :---------: | :------------------: |
|   89.38 -> 99.67   | 1000 + 1121 |        15.72         |

### s38584_com.bench

* original ATPG  

| fault coverage (%) | \# patterns | total CPU time (sec) |
| :----------------: | :---------: | :------------------: |
|                    |             |                      |

* random pattern generation + original ATPG 

| fault coverage (%) | \# patterns | total CPU time (sec) |
| :----------------: | :---------: | :------------------: |
|   89.02 -> 95.57   | 1000 + 593  |        10.16         |

### 

## Part.e
