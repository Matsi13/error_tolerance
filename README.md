# 项目名称

## 1、功能分析

根据输入的计算核大小、HBM大小、通信单元大小得出可行的排布方式，以及排布之后的芯片尺寸

根据晶圆尺寸得出晶圆上可以排列的die的总数

评估解空间的距离

根据负载和并行方式得出大致的计算量、访存量、通信量

## 2、排布方式建模

### 2.1. 类

####  2.1.1 die：

##### 2.1.1.1 属性

长度、宽度、padding

算力、存储量、访存带宽、通信带宽

##### 2.1.1.2 成员

计算核，HBM，NOC

排布方式组合表：每个排布方式包括四个列表，依次为up, down, left, right，表示对应边上排布的功能单元。可以用宏定义的数字标识这些功能单元。

##### 2.1.1.3 成员函数

构造函数

析构函数

计算尺寸等属性的函数

#### 2.1.2 HBM

尺寸，带宽，容量，需要的padding

#### 2.1.3 计算核

尺寸，算力，需要的padding

#### 2.1.4 Noc

尺寸，带宽，需要的padding

### 2.2 函数

计算解空间距离的函数

列举排布方式的函数：BFS搜索每一条边，搜索过程中用Queue保存节点，搜到叶子节点就出队，转存到一个vector里。对vector内所有字符串分别排序，然后用unique方法去重。四条边全排列得到所有可能的die的配置。

## 3、随想：
如果可以在一个计算单元周围集成多种存储单元，应该如何修改代码呢？