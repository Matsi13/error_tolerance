# System.json

- **scheduling-policy**: (LIFO/FIFO)
    - The order we proritize collectives according based on their time of arrival. LIFO means that most recently created collectives have higher priority. While FIFO is the reverse.
    - 以字符串形式指定，所有维度共用
- **intra-dimension-scheduling**: (FIFO/SCF)
    - The order we proritize collective chunks inside each dimension. FIFO means that the least recently created collectives have higher priority. SCF means that the smallest chunk have higher priority.
    - 以字符串形式指定，所有维度共用
- **inter-dimension-scheduling**: (baseline/themis)
    - The order we proritize collective chunks across multiple dimensions. baseline means that the scheduling always uses a constant schedule for all chunks. themis means that the scheduling issues chunks to the dimension with least load.
    - 以字符串形式指定，所有维度共用
- **endpoint-delay**: (int)
    - The time NPU spends processing a message after receiving it in terms of cycles.
    - 以整数形式指定，所有维度共用
- **active-chunks-per-dimension**: (int)
    - This corresponds to the Maximum number of chunks we like execute in parallel on each logical dimesnion of topology.
    - 以整数形式指定，所有维度共用
- **preferred-dataset-splits**: (int)
    - The number of chunks we divide each collective into.
    - 以整数形式指定，所有维度共用
- **all-reduce-implementation**: (Dimension0Collective_Dimension1Collective_xxx_DimensionNCollective)
    - Here we can create a multiphase colective all-reduce algorithm and directly specify the collective algorithm type for each logical dimension. The available options (algorithms) are: ring, direct, doubleBinaryTree, oneRing, oneDirect.
    - For example, “ring_doubleBinaryTree” means we create a logical topology with 2 dimensions and we perform ring algorithm on the first dimension followed by double binary tree on the second dimension for the all-reduce pattern. Hence the number of physical dimension should be equal to the number of logical dimensions. The only exceptions are oneRing/oneDirect where we assume no matter how many physical dimensions we have, we create a one big logical ring/direct(AllToAll) topology where all NPUs are connected and perfrom a one phase ring/direct algorithm.
    - 每个维度都需要单独指定。形如`[”ring”, “ring”]`

**Note**

oneRing and oneDirect is not available for Garnet Backend in this version.

- **reduce-scatter-implementation**: (Dimension0CollectiveAlg_Dimension1CollectiveAlg_xxx_DimensionNCollectiveAlg)
    - The same as “all-reduce-implementation:” but for reduce-scatter collective. The available options are: ring, direct, oneRing, oneDirect.
    - 每个维度都需要单独指定。形如`[”ring”, “ring”]`
- **all-gather-implementation**: (Dimension0CollectiveAlg_Dimension1CollectiveAlg_xxx_DimensionNCollectiveAlg)
    - The same as “all-reduce-implementation:” but for all-gather collective. The available options (algorithms) are: ring, direct, oneRing, oneDirect.
    - 每个维度都需要单独指定。形如`[”ring”, “ring”]`
- **all-to-all-implementation**: (Dimension0CollectiveAlg_Dimension1CollectiveAlg_xxx_DimensionNCollectiveAlg)
    - The same as “all-reduce-implementation:” but for all-to-all collective. The available options (algorithms) are: ring, direct, oneRing, oneDirect.
    - 每个维度都需要单独指定。形如`[”ring”, “ring”]`
- **collective-optimization**: (baseline/localBWAware)
    - baseline issues allreduce across all dimensions to handle allreduce of single chunk. While for an N-dimensional network, localBWAware issues a series of reduce-scatters on all dimensions from dim1 to dimN-1, followed by all-reduce on dimN, and then series of all-gathers starting from dimN-1 to dim1. This optimization is used to reduce the chunk size as it goes to the next network dimensions.
    - 以字符串形式指定，所有维度共用
- local-mem-bw:
    - 以整数形式指定，所有维度共用
- boost-mode:
    - 设为0