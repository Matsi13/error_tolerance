## 4、性能评估模型

### 4.1 类

#### 4.1.1 workload

计算量，存储量，通信量

是所有神经网络的父类

#### 4.1.2 神经网络（以LLM为例）

继承workload，可以有自己的各种结构参数和优化方法，使用workload的接口返回基本的性能指标

### 4.2 函数

输入架构参数和workload，返回性能。如果capacity不足，惩罚项为访问片下存储的时间。





## 5、随想：

如果可以在一个计算单元周围集成多种存储单元，多种通信单元，应该如何修改代码呢？

如果有一项性能指标无法满足，那么怎样建模惩罚项以模拟实际情况呢？

计算核内部加上SRAM？

## 6、case study

以llama中attention模块为例，分析其推理和训练过程中的计算访存通信量。（每个符号的具体含义见代码）

### 6.1 计算量

使用kv\_cache技术（https://blog.csdn.net/scgaliguodong123_/article/details/143749176）

#### 6.1.1 Norm

https://blog.csdn.net/yjw123456/article/details/138139970

单token计算量：

RMSNorm：$d_{model} * 4$

LayerNorm：$d_{model} * 8$

prefill计算量:

RMSNorm:  $prompt\_len * d_{model} * 4$

LayerNorm: $prompt\_len * d_{model} * 8$

decode计算量：设当前生成到第$x$个token，计算量为

RMSNorm：$d_{model} * 4$

LayerNorm：$d_{model} * 8$

$x=prompt\_len + 1, ..., prompt\_len + output\_len$，求和，得到整个decode阶段的计算量：

RMSNorm:  $output\_len * d_{model} * 4$

LayerNorm: $output\_len * d_{model} * 8$

### 6.1.2 Linear_0

矩阵大小为$d_{model}\times d_{model}$

单token计算量：$d_{model} * d_{model} * 2$（2代表一次乘法和一次加法）

prefill计算量：$d_{model} * d_{model} * prompt\_len * 2$

decode计算量：设当前生成到第$x$个token，计算量为$d_{model} * d_{model} * 2$

$x=prompt\_len + 1, ..., prompt\_len + output\_len$，求和，得到整个decode阶段的计算量：

$d_{model} * d_{model} * output\_len * 2$

### 6.1.3 QKV

矩阵大小依次为$W_Q:d_{model} * d_q * head\_num * 2, W_K:d_{model} * d_k * head\_num * 2, W_V:d_{model} * d_v * head\_num * 2$，输入向量尺寸为$d_{model}$

单token计算量依次为：$d_{model} * d_q * head\_num * 2, d_{model} * d_k * head\_num * 2, d_{model} * d_v * head\_num * 2$

prefill计算量依次为：$prompt\_len * d_{model} * d_q * head\_num * 2, prompt\_len * d_{model} * d_k * head\_num *2, prompt\_len * d_{model} * d_v * head\_num * 2$

decode计算量：设当前生成到第$x$个token，计算量依次为：$d_{model} * d_q * head\_num * 2, d_{model} * d_k * head\_num * 2, d_{model} * d_v * head\_num * 2$

$x=prompt\_len + 1, ..., prompt\_len + output\_len$，求和，得到整个decode阶段的计算量：

$d_{model} * d_q * head\_num * output\_len * 2, d_{model} * d_k * head\_num * output\_len * 2, d_{model} * d_v * head\_num * output\_len * 2$

### 6.1.4 RoPE

https://www.zhihu.com/tardis/bd/art/647109286

假设输入的序列长度为$x$，则RoPE的计算量依次为$x * 9 * d_q * head\_num， x * 9 * d\_k * head\_num$

prefill阶段计算量依次为$prompt\_len * d_q * head\_num * 9, prompt\_len * d_k * head\_num * 9$

decode阶段，设当前生成到第$x$个token，计算量依次为$x * 9 * d_q * head\_num， x * 9 * d_k * head\_num$

$x=prompt\_len + 1, ..., prompt\_len + output\_len$，求和，得到整个decode阶段的计算量：

$9 * d_q * head\_num * (2 * prompt\_len + 1 + output\_len) * output\_len / 2, 9 * d_k * head\_num * (2 * prompt\_len + 1 + output\_len) * output\_len / 2 $

### 6.1.5 Attention Score

使用kv\_cache技术（https://blog.csdn.net/scgaliguodong123_/article/details/143749176）

假设输入序列的长度为$x$，则K的尺寸为$x * d_k * head\_num $， V的尺寸为$x * d_v * head\_num $， （$d_k = d_v$），生成的Attention Score的尺寸为$x * x * head\_num$，计算量为$x * d_k * x * head\_num * 2$

prefill阶段的计算量为$prompt\_len^2 * d_k * head\_num *2$

decode阶段使用KV_cache技术，设当前生成到第$x$个token，则前面已有$prompt\_len + x - 1$个token，Q的尺寸为$x * d_q$，K的尺寸为$(prompt\_len + x - 1) \times d_k \times head\_num$， V的尺寸为$(prompt\_len + x - 1) \times d_v \times head\_num$， 已生成的Attention Score的大小为$(prompt\_len + x - 1) \times (prompt\_len + x - 1) \times head\_num$, 需要重新计算的是Attention Score的最后一行（最后一列会被mask掉，不用重新算）。所以这里需要的计算是大小为$(prompt\_len + x) \times d_k \times head\_num$的K矩阵乘上大小为$d_k \times head\_num$的q向量，总计算量为$(prompt\_len + x) * d_k * head\_num * 2 $， 

$x = 1, ..., output\_len$， 求和得到decode阶段的总计算量

$d_k * head\_num * (2 * prompt\_len + output\_len + 1) * output\_len $

### 6.1.6 Softmax

使用kv\_cache技术（https://blog.csdn.net/scgaliguodong123_/article/details/143749176）

softmax的计算量为：除以$\sqrt{d_k}$，求最大值，作差，求指数，求和，除以总和，共计6次

prefill阶段的计算量为：$prompt\_len^2 * head\_num * 6 $

decode阶段使用KV cache技术，只有最后一行需要重新做softmax。设当前生成到第$x$个token，则本行长度为$x + prompt\_len$，需要的计算量为$6 * (x + prompt\_len) * head\_num $

$x = 1, ..., output\_len$，求和，得到decode阶段的总计算量

$3 * (2 * prompt\_len + 1 + output\_len) * output\_len * head\_num $

### 6.1.7 dot product with V

prefill阶段的计算量为$prompt\_len * prompt\_len * d_v * head\_num * 2$

decode阶段，设当前生成到第$x$个token， 则输出的Attention Score矩阵的尺寸为$x \times (prompt\_len + x) \times head\_num$，其中只有尺寸为$1 \times (prompt\_len + x)$最后一行需要与K与尺寸为$(prompt\_len + x) \times d_v \times head\_num $的向量相乘，计算量为$(prompt\_len + x) * d_v * head\_num * 2 $

$x = 1, ..., output\_len$ ，求和得到decode阶段的总计算量

$d_v * head\_num * (2 * prompt\_len + 1 + output\_len) * output\_len $

### 6.1.8 linear_1

prefill阶段的计算量为$prompt\_len * d_v * d\_hidden * 2$

decode阶段，设当前生成到第$x$个token，则输出的V的尺寸为
