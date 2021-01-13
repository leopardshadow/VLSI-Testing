<h1 align=center> VLSI testing - Lab 1 & 2 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

* pre_norm: Noscan vs. Scanned (DC+TMAX)

|            |  Area  | Power (W) | Fault count | Coverage (collapsed) (%) | ATPG Run Time(s) | Pattern |
| ---------- | :----: | :-------: | :---------: | :----------------------: | :--------------: | :-----: |
| NonScanned | 98072  |   260u    |    4011     |          98.05           |     7137.43      |   279   |
| Scanned    | 105072 |   271u    |    4415     |          99.34           |       0.72       |   159   |

* s38584_seq: Noscan vs. Scanned (DC+TMAX)

|            |  Area   | Power (W) | Fault count | Coverage (collapsed) (%) | ATPG Run Time(s) | Pattern |
| ---------- | :-----: | :-------: | :---------: | :----------------------: | :--------------: | :-----: |
| NonScanned | 1081468 |  1.8636m  |    55680    |          10.75           |     89754.29     |    6    |
| Scanned    | 1223322 |  5.8111m  |    42188    |           100            |       1.32       |   136   |

* different design flows for pre_norm

|  Flow   | \#Faults | Test Coverage (%) | \#Patterns | Run time (s) |
| :-----: | :------: | :---------------: | :--------: | :----------: |
| DC+TMAX |   4415   |       99.34       |    159     |     0.72     |
| DFT+FS  |   6098   |       99.73       |    192     |     5.5      |
|  DC+FS  |   6012   |       99.85       |    195     |     4.4      |

