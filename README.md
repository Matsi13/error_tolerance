# 项目名称

## 1、功能分析

### 1.1 可行解探索

根据输入的计算核尺寸、存储单元尺寸、通信单元尺寸得出可行的排布方式，据此计算出单晶粒尺寸，进而得出在指定晶圆尺寸下可实现的晶圆性能

### 1.2 误差分析

根据1.1中建立的解空间，分析给定的仿真器误差下，可能取代当前最优解的其他排布方案

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



## 3、随想：
如果可以在一个计算单元周围集成多种存储单元，多种通信单元，应该如何修改代码呢？

如果有一项性能指标无法满足，那么怎样建模惩罚项以模拟实际情况呢？