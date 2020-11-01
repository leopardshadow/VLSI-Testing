<h1 align=center> VLSI testing - Assignment 3 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

[TOC]

## Algorithm





## Test Cases

parallel logic simulation with different input pack number.

### c17.bench (5 inputs / 2 outputs)

* c17.input (24 cases)

|         pack number         |  1   |  4   |  8   |  16  |
| :-------------------------: | :--: | :--: | :--: | :--: |
|          CPU time           |      |      |      |      |
|      Max. memory (MB)       |      |      |      |      |
| Total gate evaluations num. | 216  |  54  |  27  |  18  |
| Avg. gate evaluations num.  |      |      |      |      |

The total gate evaluation numbers are calculated as follows.

* n : number of cases (it is 24 here)
* x : input pack number (1, 4, 8, 16 respectively)
* c : number of evaluations of for a single pattern

$$
total~~gate~~evaluations~~num. = c * \lceil \frac{n}{x} \rceil
$$



### c7522.bench

* c7552-10000.input (10000 input patterns)

|                            |    1     |    4     |    8     |    16    |
| :------------------------: | :------: | :------: | :------: | :------: |
|          CPU time          | 0.473004 | 0.171178 | 0.121689 | 0.102852 |
|        Max. memory         |   2496   |   2500   |   2508   |   2508   |
| avg. gate evaluations num. |          |          |          |          |







|          |  1   |  4   |  8   |  16  |
| :------: | :--: | :--: | :--: | :--: |
| CPU time |      |      |      |      |
|  Memory  |      |      |      |      |





## Discussion







## 





## Build

```
make
./ atpg ...
```

