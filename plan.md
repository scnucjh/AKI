- [x] 画算法草图
- [x] 写算法伪代码
- [x] 处理数据格式
- [x] 编程实现
- [ ] 写一篇简短的pdf报告 


## 简单描述

首先是给RECTANGLE密钥的每个状态都设置成3个状态

0. 进S盒之前
1. 进S盒之后
2. 移位及异或后

于是每个密钥比特可以表示成
$w_{s,r,m}$ s表示状态，$r$表示轮数，$m$表示比特编号。

接下来要做的事情是列出所有可以列出的表达式
每一条的形式都是这样的：

$$a,b,c,d, \Rightarrow e$$

或
$$ a,b \Rightarrow c$$

列出之后，把那些AKI的比特对应的$w_{0,r,m}$置1，其它置0.
每次尽量去找一条方程来更新，直到无法更新。

最后检查是否所有的路径比特的$w_{0,r,m}$都是1。

## 算法伪代码

### 主过程

```
bool IS_AKI = read_AKI()
bool IS_PATH = read_PATH()

vector<Fun> funs= gen_fun() 

while(1) {
    bool update = 0;
    for(auto element:funs) {
        if(element.all_left_can_be_deduced()) {
            if(!element.all_right_can_be_deduced()) {
                element.set_all_right_be_deduced();
                update = 1;
            }

        }
    }
    if(!update) break;
}

check();
```

### gen_fun()

```
function id(x,y) 
    return x*32+y

function gen_fun() 
    vector<fun> 
    for r in [0,5]
        for i in [0,3]
            for j in [8,32)
                     [r,0,id(i,j)]  <-> [r,0,id(i,j)]

        for j in [0,7]
            [r,0,id(0,j)][r,0,id(1,j)]
            [r,0,id(2,j)][r,0,id(3,j)]
            <->
            [r,1,id(0,j)][r,1,id(1,j)]
            [r,1,id(2,j)][r,1,id(3,j)]

        for j in [0,31]
            [r,1,id(0,j-8)][r,1,id(1,j)] -> [r,2,id(0,j)]
            [r,1,id(0,j-8)][r,2,id(0,j)] -> [r,1,id(1,j)]
            [r,1,id(1,j)]  [r,2,id(0,j)] -> [r,1,id(0,j-8)]

            [r,1,id(2,j)] <-> [r,2,id(1,j)]

            [r,1,id(2,j-16)][r,1,id(3,j)] -> [r,2,id(2,j)]
            [r,1,id(2,j-16)][r,2,id(2,j)] -> [r,1,id(3,j)]
            [r,2,id(2,j)][r,1,id(3,j)]    -> [r,1,id(2,j-16)]

            [r,1,id(0,j)] <-> [r,2,id(3,j)]
        
        for i in [0,3]
            for j in [0,31]
                [r,2,id(i,j)] <-> [r+1,0,id(i,j)]
                   
```


### 编程实现：

- [x] Fun类设计
- [x] gen_fun()
- [x] check()
- [x] 主过程设计


# 计算部分

## 伪代码

```

aki_set = {}
funs = gen_fun()

r = 0
while r < ROUNDS
    bool update = false;
    DEDUCE()
    for j in [0,127]
        if IN_PATH[r][j] and !deduce_set[r][0][j]
            aki_set.insert( subkey[r][j] )
            deduce_set[r][0][j] = 1
            update = true
            break
    if(!update) r++

    
```


## RECTANGLE-128 AKI的验证与一种新的计算方法
在本报完成之前，对闫海伦论文《New observation on the key schedule of RECTANGLE》中RECTANGLE-128 6轮下的AKI为124这一数据进行了验证。在得到数据后，尝试使用数据中AKI具体的比特集合，推导出整个路径上每个比特的值。如果能够推导出整个路径上的所有密钥，则说明该AKI集合是正确的。实验以数据中RECTANGLE-128 6轮，且第6轮在路径上的比特是0号比特为例，进行了验证。验证结果显示可以推导出整个路径上的比特。

随后又受验证时采用方法的启发，设计了一种新的AKI的计算方法，用路径上的比特来计算AKI，结果同样可以算出AKI集合为124个。在阅读作者的github项目后，发现在计算AKI的过程中使用了网络流算法，本报告使用的方法与原方法不同，但能得到相同的结果。本报告的方法的正确性还不能被严谨证明。

### 验证过程的文字叙述
把Key Schedule中每一个位置的比特编号为3个状态：0状态，进S盒之前和；1状态，进S盒之后；2状态，进行行移位和异或操作之后。

每个状态的密钥比特用一个布尔变量表示：$K_{r,s,m}$, 表示第r轮的第m个比特，处于s状态时是否可以被推导计算出来。

那么可以列出一系列的推导式，例如对于S盒操作，有：
$$ K_{r,0,m} , K_{r,0,m+32}, K_{r,0,m+64}, K_{r,0,m+96}   \Rightarrow 
    K_{r,1,m} , K_{r,1,m+32}, K_{r,1,m+64}, K_{r,1,m+96}
$$
只有8列进S盒，所以列出16条这样的推导式（右边也可以推导左边）。

除了最左边8列，其他列都是由状态0和状态1互相推导的。于是列出类似如下的推导式。
$$ K_{r,0,m}  \Rightarrow K_{r,1,m} $$
$$ K_{r,1,m}  \Rightarrow K_{r,0,m} $$

从1状态到2状态，经历了行移位和异或操作，看这两个操作结合起来，得到一些推导式，例如：
$$ K_{r,0,24} , K_{r,1,0}  \Rightarrow K_{r,2,0} $$ 
对于异或操作，其中两个比特可以被推导，则第三个比特也可以被推导，于是有：
$$ K_{r,0,24} , K_{r,2,0}  \Rightarrow K_{r,1,0} $$ 
$$ K_{r,2,0} , K_{r,1,0}  \Rightarrow  K_{r,0,24}$$ 
这样由状态1到状态2，可以列出3*96 = 288个推导式。

最后，当前第r轮的状态2的每个变量的取值和r+1轮的每个0状态的比特的变量的取值都是相同的。
对0<=m<96，有
$$ K_{r,2,m} \Rightarrow  K_{r+1,0,m} $$
$$ K_{r+1,0,m} \Rightarrow  K_{r,2,m} $$

要验证6轮的AKI集合是否正确，首先列出所有6轮下这样的推导式，然后，对于AKI中的每个比特
$W_{r,j}$（第r轮的第j个子密钥），令$K_{r,0,j}$的取值为真。然后开始进行推导过程。具体地说，遍历所有推导式，如果能够找到这样一条推导式，其左边的变量取值全真，右边变量取值不全真，这右边的变量可以被推导为全真。一直重复这个过程，知道无法更新为止。最后进行检查，看是否路径上的每个比特$PATH_{r,j}$，都有$K_{r,0,j}为真$。

### 验证过程的伪代码
## 算法伪代码

### 主过程

```
bool IS_AKI = read_AKI()
bool IS_PATH = read_PATH()

vector<Fun> funs= gen_fun() 

while(1) {
    bool update = false;
    for(auto element:funs) {
        if(element.all_left_can_be_deduced()) {
            if(!element.all_right_can_be_deduced()) {
                element.set_all_right_be_deduced();
                update = true;
            }

        }
    }
    if(!update) break;
}

check();
```

### gen_fun()

```
function id(x,y) 
    return x*32+y

function gen_fun() 
    vector<fun> 
    for r in [0,5]
        for i in [0,3]
            for j in [8,32)
                     K[r,0,id(i,j)]  <-> K[r,0,id(i,j)]

        for j in [0,7]
            K[r,0,id(0,j)][r,0,id(1,j)]
            [r,0,id(2,j)][r,0,id(3,j)]
            <->
            K[r,1,id(0,j)][r,1,id(1,j)]
            [r,1,id(2,j)][r,1,id(3,j)]

        for j in [0,31]
            K[r,1,id(0,j-8)][r,1,id(1,j)] -> K[r,2,id(0,j)]
            K[r,1,id(0,j-8)][r,2,id(0,j)] -> K[r,1,id(1,j)]
            K[r,1,id(1,j)]  [r,2,id(0,j)] -> K[r,1,id(0,j-8)]

            K[r,1,id(2,j)] <-> K[r,2,id(1,j)]

            K[r,1,id(2,j-16)][r,1,id(3,j)] -> K[r,2,id(2,j)]
            K[r,1,id(2,j-16)][r,2,id(2,j)] -> K[r,1,id(3,j)]
            K[r,2,id(2,j)][r,1,id(3,j)]    -> K[r,1,id(2,j-16)]

            K[r,1,id(0,j)] <-> K[r,2,id(3,j)]
        
        for i in [0,3]
            for j in [0,31]
                K[r,2,id(i,j)] <-> K[r+1,0,id(i,j)]
                   
```
结果显示数据中的124个AKI比特可以推导出路径上的全部比特，并且去掉这124个比特中的任何一个比特，都不能推导出路径上的全部比特。

### RECTANGLE-128 AKI的计算方法

在对RECTANGLE-128进行验证的过程中，想到了利用类似的思路来从路径上的比特集合求AKI的方法。
该算法的过程如下：初始化一个AKI集合为空集。首先还是要生成r轮下的所有上述推导式，之后，轮数从0轮到r轮依次枚举，每次枚举当前轮下路径上的一个比特，看其当前是否可以被已经认为是AKI的那些比特推导出来，如果不能，将这个比特视为可推导，并且加入AKI的集合中。重复这个过程，直至路径上的每个比特都可以被推导，AKI集合里面的元素就是最终的答案。

### 伪代码

```

aki_set = {}
funs = gen_fun()

r = 0
while r < ROUNDS
    bool update = false;
    DEDUCE()
    for j in [0,127]
        if IN_PATH[r][j] and !deduce_set[r][0][j]
            aki_set.insert( subkey[r][j] )
            deduce_set[r][0][j] = 1
            update = true
            break
    if(!update) r++

```

### 复杂度分析

每次推导只需要遍历所有推导式，推导式的数量是$O(nr)$级别的，其中，r是轮数,n是密钥长度。对路径上的每个比特都要进行一次推导，路径上的比特的数量也是$O(nr)$级别的，所以总的时间复杂度是$O(r^{2}n^{2})的$


### 代码
验证和计算的代码均使用c++11实现，放在github上。

