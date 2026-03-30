# 使用指南

# 1. chakra的使用

```bash
conda activate astra-env
cd ./astra-sim/extern/graph_frontend/chakra/
chakra_converter Text \
    --input ../../../examples/text_converter/text_workloads/Resnet50_DataParallel.txt \
    --output ../../../examples/text_converter/text_workloads/Resnet50_DataParallel \
    --num-npus 32 \
    --num-passes 1
cd ../../../examples/text_converter/text_workloads/
```

生成的文件在`~/astra-sim/examples/text_converter/text_workloads`文件夹下。注意，生成文件的数量`--num-npus`所指定的数量相同。

注释：目前服务器上的chakra只支持DATA并行模式。

在chakra文件夹下的readme.md中，可以找到text_workloads的具体格式说明

# 2. astra-sim的使用

将chakra生成的文件下载下来，与修改过的系统配置一起上传到虚拟机的`~/workload/astra-sim/configs` 文件夹下。然后在虚拟机中运行以下指令：

```bash
./build/astra_analytical/build/bin/AstraSim_Analytical_Congestion_Unaware \
  --workload-configuration=./configs/config2/workload_et/Resnet50_DataParallel \
  --system-configuration=./configs/config2/system.json \
  --network-configuration=./configs/config2/network.yml \
  --remote-memory-configuration=./configs/config2/remote_memory.json
```

ns3后端的运行方法如下：

```bash
 cd ~/workspace/astra-sim
    ./extern/network_backend/ns-3/build/scratch/ns3.42-AstraSimNetwork-default \
        --workload-configuration=./configs/config3/workload_et/Resnet50_DataParallel  \
        --system-configuration=./configs/config3/system.json  \
        --network-configuration=./configs/config3/config.txt   \
        --remote-memory-configuration=./configs/config3/remote_memory.json \
        --logical-topology-configuration=./configs/config3/sample_8nodes_1D.json   \
        --comm-group-configuration=\"empty\"
```

注意config.txt中的文件路径需要修改为绝对路径。

# 3. astra-sim analytical详解

[System.json](https://www.notion.so/System-json-2181c04cd5cd80a9a9c6dc21a58c790a?pvs=21)

[Network.yml](https://www.notion.so/Network-yml-2181c04cd5cd80c883cdf97d4a833e7d?pvs=21)

[remote_memory.json](https://www.notion.so/remote_memory-json-2181c04cd5cd80f4a7e7dbbeddcc237d?pvs=21)