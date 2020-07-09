## 1. Data Description

This chapter has two section, the first section describes the ```Instance graph``` which includes two parts, and the second section represents the ```Signal graph``` which also includes two parts.  

### 1.1 Instance graph

The first line is a number **N**, which represents the number of ```Instance```. And N lines are followed. For the each line, 
**3** items are expected. They represtens ```Instance ID```,```Instance Name``` and ```Moudule Name``` respectively.

The following line has **Only one** number of **M**, which representes the number of **edge**. **M** lines are followed, each line has **2** numbers : **u** and **v**. They representes the **Instance ID** and it's Father's respectively.

### 1.2 Signal graph

The first line is a number **N**, which represents the number of vertices. And N lines are followed. For the each line, **8** items are expected. They represents ```Id```,```Instance ID```,```Name```,```Type```,```CppType```,```Width```,```Dir``` and ```Ext``` respectively. 
The following line has **Only one** number of **M**, which representes the number of **edge**. **M** lines are followed, each line has **2** numbers : **u** and **v**. They representes the **sink** and **source** respectively.

Tips: 

- The ```ID``` and ```Name``` is **unique**.
- The ```Type``` is the origin type defined in the firrtl, if you want to learn more, plz read the **firrtl spec**.
- More details will be introduced in other document, if you have a issues, plz let me know.

## Type
```
Node	:42890
Mux		:16853
UIntLiteral		:15964
Eq		:8621
Port	:4875
Bits	:4143
And		:2692
Reg		:2331
Or		:1139
Cat		:985
ValidIf	:609
Add		:326
Tail	:301
AsUInt	:226
Not		:194
Memen	:164
Memdata	:164
Memclk	:164
Memaddr	:164
Neq		:130
Pad		:68
Memmask	:64
Lt		:44
Geq		:40
Xor		:35
Dshr	:17
Dshl	:15
Sub		:13
Gt		:12
Shl		:11
Shr		:4
AsSInt	:4
Mul		:2
Leq		:1
```
## Width

```
83
0	10702
1	56098
2	3439
3	4788
4	4655
5	1487
6	528
7	1095
8	1350
9	148
10	188
11	56
12	474
13	21
14	30
15	792
16	1167
17	209
18	262
19	371
20	31
21	125
22	11
23	28
24	43
25	70
26	241
27	24
28	110
29	36
30	35
31	725
32	1795
33	14
35	14
36	152
37	180
38	460
39	2501
40	6
41	12
42	8
43	12
46	8
47	6
48	5
51	22
52	32
54	10
55	18
56	35
57	4
58	20
59	25
60	54
61	52
62	156
63	856
64	6997
65	39
68	14
70	8
71	16
72	86
76	2
78	6
84	4
86	2
87	33
91	6
121	160
124	2
126	4
127	6
128	5
129	5
130	6
182	16
183	4
256	12
274	2
408	32
2599	2
```
## vcd files
```
1597322 <- Mcycle
783385 <- Minstret
    0 <- MultiCommit
83239 <- MimemStall
422261 <- MaluInstr
186742 <- MbruInstr
171382 <- MlsuInstr
3001 <- MmduInstr
    0 <- McsrInstr
121535 <- MloadInstr
    0 <- MmmioInstr
    0 <- MicacheHit
    0 <- MdcacheHit
2277 <- MmulInstr
36932 <- MifuFlush
119234 <- MbpBRight
9532 <- MbpBWrong
18871 <- MbpJRight
2538 <- MbpJWrong
8780 <- MbpIRight
23470 <- MbpIWrong
2925 <- MbpRRight
1392 <- MbpRWrong
    0 <- Ml2cacheHit
    0 <- Custom1
2984 <- Custom2
    0 <- Custom3
    0 <- Custom4
    0 <- Custom5
6548 <- Custom6
    0 <- Custom7
    0 <- Custom8
2055  <- MrawStall
514846 <- MexuBusy
250195 <- MloadStall
146832 <- MstoreStall
853497 <- ISUIssue
853497 <- ISU1Issue
    0  <- ISU2Issue
375694 <- Src2NotReady
192054 <- Dst2Conflict
477803 <- Inst2NotALU
160655 <- Inst2NoReady

HIT GOOD TRAP at pc = 0x80004638
total guest instructions = 783385
instrCnt = 783385, cycleCnt = 1597322, IPC = 0.490436
Guest cycle spent: 1597323
Host time spent: 105087ms
```