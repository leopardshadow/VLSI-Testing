<h1 align=center> VLSI testing - Lab 1 & 2 </h1>

<h3 align="right"> 309510133 - Cheng-Cheng Lo </h3>

* pre_norm

|            |  Area  | Power (W) | Fault count | Coverage (collapsed) (%) | ATPG Run Time(s) | Pattern |
| ---------- | :----: | :-------: | :---------: | :----------------------: | :--------------: | :-----: |
| NonScanned | 98072  |   260u    |             |                          |                  |         |
| Scanned    | 105072 |   271u    |    4415     |          99.34           |       0.72       |   159   |

* s38584_seq

|            |  Area   | Power (W) | Fault count | Coverage (collapsed) (%) | ATPG Run Time(s) | Pattern |
| ---------- | :-----: | :-------: | :---------: | :----------------------: | :--------------: | :-----: |
| NonScanned | 1081468 |  1.8636m  |             |                          |                  |         |
| Scanned    | 1223322 |  5.8111m  |    42188    |           100            |       1.32       |   136   |

* different design flows for pre_norm

|  Flow   | \#Faults | Test Coverage (%) | \#Patterns | Run time (s) |
| :-----: | :------: | :---------------: | :--------: | :----------: |
| DC+TMAX |   4415   |       99.34       |    159     |              |
| DFT+FS  |          |                   |            |              |
|  DC+FS  |          |                   |            |              |

