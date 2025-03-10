# 项目名称

## 1、功能分析

### 1.1 可行解探索

根据输入的计算核尺寸、存储单元尺寸、通信单元尺寸得出可行的排布方式，据此计算出单晶粒尺寸，进而得出在指定晶圆尺寸下可实现的晶圆性能

### 1.2 误差分析

根据1.1中建立的解空间，分析给定的仿真器误差下，可能取代当前最优解的其他排布方案

### 1.3 性能分析

对于一些特定结构的计算任务（例如不同参数的transformer类，加上一些已知的input长度和预期output长度），使用公式分析其计算访存通信需求，进而选择最接近于这些指标需求的空间架构

计算访存通信需求会受到并行方式和重计算等优化手段的影响，所以每个应用的需求是一系列散点。这些都是总的计算访存通信需求，是不考虑数据依赖的情况，即所有的操作可以完全交织。

思考：

怎样考虑具体的依赖关系？如果有规律，或许这些依赖关系也可以被简单建模

或者可以给出这种解析公式的误差上限，将误差范围内的架构也包括进来



## 2、排布方式建模

### 2.1. 类

#### 2.1.1 计算核（Compute）

尺寸，算力，需要的padding

#### 2.1.2 存储单元（Memory）

尺寸，带宽，容量，需要的padding

#### 2.1.3 通信单元（Communication）

尺寸，带宽，需要的padding

#### 2.1.4 晶粒（Die）：

用晶粒描述排列方式

##### 2.1.1.1 成员

尺寸、padding、算力、存储量、访存带宽、通信带宽

计算核，存储单元，通信单元

排布方式组合表：每个排布方式包括四个字符串，依次为up, down, left, right，表示对应边上排布的功能单元。用宏定义的字符标识这些功能单元。

##### 2.1.1.2 成员函数

update()：根据计算存储通信单元的排布计算尺寸和性能指标

#### 2.1.5  晶圆（wafer）

用晶圆描述解空间中的解

##### 2.1.5.1 成员：

尺寸，晶粒之间的padding，总算力，总存储，总访存带宽，总通信带宽

使用的晶粒，晶粒行数，晶粒列数

##### 2.1.5.2 成员函数

print()：依次打印总算力，总存储，总访存带宽，总通信带宽

update()：根据晶粒和晶圆尺寸计算晶圆上晶粒的行数、列数和晶圆的性能指标

### 2.2 函数

#### 2.2.1 列举排布方式

Permutation(Compute& Compute_unit, Memory& Memory_unit, Communication& Communication_unit, float die_padding, list\<Wafer\>& result, float relaxation, float wafer_length, float wafer_width, Threshold threshold)

输入计算、存储、通信单元的参数，晶圆参数（尺寸，padding），设计参数（relaxation，最低性能指标）

注：relaxation指每条边上的存储和通信单元总长度可以略微超过计算核的边长

输出可行的晶粒配置

#### 2.2.2 搜索可能的最优解

Possible_optimal(queue\<Wafer\> solutions, Wafer& optimal, simulation_error& error, queue\<Wafer\>& possible_optimals)

输入当前解空间，仿真器找到的最优解，仿真器可能产生的最大误差

输出误差容限内所有可能的最优解

#### 2.2.3 从手动配置的文件中读入参数

Scan(list\<Compute\>& compute_configs, list\<Memory\>& memory_configs, list\<Communication\>& communication_configs, string input_path, Config& input_config)

输入文件中依次列出晶圆参数、设计指标、可用的计算存储通信核数量和具体参数

scan函数读取文件内容

#### 2.2.4 批量生成配置参数

Generate(list\<Compute\>& compute_configs, list\<Memory\>& memory_configs, list\<Communication\>& communication_configs, string input_path, Config& input_config)

晶圆参数

设计指标

计算核：首先列出可用的计算核系列数量，每个系列包括以下参数：每面积可提供的算力，长度的迭代区间（最小值、迭代步长、最大值），宽度的迭代区间，padding

存储单元：首先列出可用的存储单元系列数量，每个系列包括以下参数：每面积可提供的存储量，单位长度可提供的访存带宽，长度的迭代区间，宽度的迭代区间，padding

通信单元：首先列出可用的通信单元系列数量，每个系列包括以下参数：单位长度可提供的通信带宽，长度的迭代区间，宽度的迭代区间，padding

## 3、负载分析

### 3.1 decode

假设即将运行的任务是LLM，目前的序列长度$seq=200$，每个词向量的维度$d_{model}=512$，attention head数$h=8$， 矩阵$W^Q:d_{model}\times d_k, W^K:d_{model}\times d_k, W^V:d_{model}\times d_v$ ，其中$d_k=d_v=64$， $Q=XW^Q,K=XW^K,V=XW^V$， Attention层输出为$Z:1\times d_{v}$，$Z=softmax(\frac{QK^T}{\sqrt{d_k}})V$ 。FFN有两层，权重矩阵为$W_1:d_{model}\times hidden, W_2:hidden\times d_{model}$，其中$hidden=2048$。两个FFN层之后都有激活函数。可以认为除了KV_cache之外，其他权重一直保存在计算芯片的SRAM上，无需搬运。

对于新的token，生成$QKV$的计算量为$3\times2\times d_{model}\times d_k \times h=1.5M$，生成$Z$的计算量为$5\times d_v\times h=2.5K$，FFN层的计算量为$2\times d_{model}\times hidden + hidden\times2 + 2\times hidden\times d_{model} + d_{model}\times 2=4M$，总计算量约为$5.5M$

考虑到已有KV_cache的decode情况，此时的矩阵$K:(d_k\times h)\times seq$，矩阵$V:seq\times(d_k\times h)$，生成$Z$的计算量为$5 \times d_{model} \times seq \times d_{model} + 2 \times d_{model} \times d_{model} = 250M$，需要读取的KV_cache约为$seq\times d_{model} \times 2\times 2=400KB$，

计算访存比$R_d=\frac{250\times1024}{400}\times \frac{1}{2250}=0.28$

通用公式：

一个transformer block一次前向推理的计算量为$C_{f} (seq)= 6 d_{model} d_k h + 5 d_v h + 2 d_{model} hidden + 2 hidden + 2 hidden d_{model} + 2 d_{model} + 5 d_{model} seq d_{model} + 2 d_{model}^2 $，

访存量为$M_{f}(seq)=4seq d_{model} $，

通信量为$N_{f}=seq*d_{model}$

假设每个die上包含$l$个串联的 transformer block，生成了一个长度为$S$的输出，那么总计算量为$l*S*C_{f}(S/2)$，总访存量为$l*S*M_f(S/2)$，总通信量为$S/2*N_f$

考虑反向传播过程。假设每$R$个transformer block设置一个检查点，每个检查点保留整个transformer block的中间值（不含输出）。一个transformer block的中间值总量为$M_{ime}=S*(d_{model}+2d_k*h+d_v*h+d_{model}+hidden+d_{model})$。LLM总共包含$L$个transformer block，当前die负责计算的是第$i\to i+l-1$层。检查点设置在第$i, i+R, i+2R, ......$层。则反向传播的计算量为$3*l*C_{f}(S)*\frac{R-1}{R}$，访存量为$M_{ime}*l/R$ 。

使用1F1B策略，则流水线平稳运行时，计算压力为前向+反向，存储压力为$M_{ime}*(L-i+l/2-R/2)*(l/R)$，通信压力为前向+后向，访存压力为前向+后向。



### 3.2 prefill

考虑prefill情况，prompt长度为20，则计算量为$20*5.5M=110M$，计算访存比 $R_d=\frac{110}{5.5}\times \frac{1}{2250}=0.28$

### 3.3 training

![image-20240906102326999](C:\Users\Matsi\AppData\Roaming\Typora\typora-user-images\image-20240906102326999.png)

以LLaMA为例，主要计算量在于其32层transformer block，每个transformer block有32个head。

每个transformer block的权重总量约6MB，为了训练而保留的权重梯度为12MB，前向推理计算总量为$7.89e^{-2}$TFLOPS，反向传播计算总量相同。

决定访存总量的是重计算方式，1MB的存储对应$7.89e^{-2}$TFLOPS计算量。

### 3.4 权重同步

考虑权重同步过程中的通信模式。

假设16个die，每4个die保存一个完整网络，排布方式是2X2，则通信模式为：

![wafer scale_00](D:\_实验室\2025春\PAPER\wafer scale_00.jpg)

