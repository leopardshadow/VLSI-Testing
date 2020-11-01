<h1 align=center> VLSI testing - Assignment 3 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

[TOC]

## Different Pack Numbers

parallel logic simulation with different input pack number.

### c17.bench

* c17.input (24 cases)

|         pack number         |    1     |    4     |    8    |   16    |
| :-------------------------: | :------: | :------: | :-----: | :-----: |
|          CPU time           | 0.000138 | 0.000181 | 7.1e-05 | 8.6e-05 |
|      Max. memory (MB)       |   828    |   828    |   800   |   828   |
| Total gate evaluations num. |   216    |    54    |   27    |   18    |
| Avg. gate evaluations num.  |          |          |         |         |

The total gate evaluation numbers are calculated as follows.

* n : number of cases (it is 24 here)
* x : input pack number (1, 4, 8, 16 respectively)
* c : number of gate evaluations for a **single pattern**

$$
total~~gate~~evaluations~~num. = c * \lceil \frac{n}{x} \rceil
$$

**It's a quite small test case, which each paraemeter can be easily affected by other factors (e.g. the time for parsing may account for a large part). Let's see the example below.**

### c7522.bench

* c7552-10000.input (10000 input patterns)

|         pack number         |    1     |    4     |    8    |    16    |
| :-------------------------: | :------: | :------: | :-----: | :------: |
|       total CPU time        | 6.34783  | 1.63688  | 0.85108 | 0.462134 |
|      Max. memory (MB)       |   2572   |   2556   |  2568   |   2580   |
| Total gate evaluations num. | 57870000 | 14467500 | 7233750 | 3616875  |
| Avg. gate evaluations num.  |          |          |         |          |

In this example, n = 10000, x = 1, 4, 8, 16 for different cases and c = 5787. Total number of gate evaluations is calculated as the formula above.

![image-20201101210338558](/Users/cheng/Desktop/VLSI-Testing/hw3/image-20201101210338558.png)



## Event-driven vs. Compiled Code

### c7522.bench

* c7552-10000.input (10000 input patterns)

|                  | Event-driven | compiled code |
| :--------------: | :----------: | :-----------: |
|  total CPU time  |   0.963047   |   0.429604    |
| Max. memory (MB) |     2524     |     13416     |



## Discussion





Notice that gate name may start with a number. However, it's not  valid variable name on C++. Thus, some modification has to be made to fit the requirements.

## 





## Build

```
make
./atpg -simulator c17.cc -input c17.input c17.bench
g++ c17.cc
./a.out
```

