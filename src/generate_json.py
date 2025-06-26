import os
import json

# 你是一个擅长python的程序员。我需要你帮我写一个python函数。
# 函数的输入是字符串config，输入文件路径input_path，输出文件路径output_path，输入路径下有一个名为config_wafer.txt的文件（这里的config即为函数的输入参数字符串config）。文件有许多行，每行的列数是相同的。对于每一行，python应该输出一个yml文件，命名为<config>_<行数（六位整数）>_network.yml，例如0050_001024_network.yml。
# Txt文件的前5列没有意义。第6列是整数rows，第7列是整数columns，第8-30列没有意义。第31列是float类型的mem_bandwidth，第32列是float类型的communication_bandwidth。列之间用空格隔开。
# 输出yml文件内容如下：
# 第1行：topology: [Ring, Ring]
# 第2行：npus_count: [columns, rows]，这里的columns和rows即为txt文件中读取到的变量，是整数
# 第3行：bandwidth: [communication_bandwidth, communication_bandwidth]，这里的communication_bandwidth是txt文件中读取到的变量。
# 第4行：latency: [50.0, 50.0]

# 很好，接下来我需要你再设计一个python函数，还是根据刚才的txt文件，这次每行输出一个json文件。Json文件中的元素如下：
# 1：”scheduling-policy”: “LIFO”
# 2: “intra-dimension-scheduling”: “FIFO”
# 3: “inter-dimension-scheduling”: “baseline”
# 4:  “endpoint-delay”: 10,
# 5: “active-chunks-per-dimnesion”:1
# 6: “preferred-dataset-splits”: 1
# 7: “all-reduce-implementation”: ["ring", “ring”],
# 8: “all-gather-implementation": ["ring", “ring”],
# 9: "reduce-scatter-implementation": ["ring", “ring”],
# 10: "all-to-all-implementation": ["ring", “ring”],
# 11:”local_mem_bw”: txt文件中读取的变量mem_bandwidth,
# 12: “boost_mode”: 0


def generate_network_json(config: str, input_path: str, output_path: str):
    """
    读取input_path下名为config_wafer.txt的文件，按行生成json文件到output_path。
    json文件命名为<config>_<行号（六位整数）>_system.json。
    json内容格式见函数说明。
    """
    input_file = os.path.join(input_path, f"{config}_wafer.txt")
    if not os.path.exists(output_path):
        os.makedirs(output_path)
    with open(input_file, 'r') as f:
        for idx, line in enumerate(f, 1):
            parts = line.strip().split()
            if len(parts) < 32:
                continue  # 跳过格式不对的行
            mem_bandwidth = float(parts[30])
            json_data = {
                "scheduling-policy": "LIFO",
                "intra-dimension-scheduling": "FIFO",
                "inter-dimension-scheduling": "baseline",
                "endpoint-delay": 10,
                "active-chunks-per-dimension": 1,
                "preferred-dataset-splits": 1,
                "all-reduce-implementation": ["ring", "ring"],
                "all-gather-implementation": ["ring", "ring"],
                "reduce-scatter-implementation": ["ring", "ring"],
                "all-to-all-implementation": ["ring", "ring"],
                "local_mem_bw": mem_bandwidth,
                "boost_mode": 0
            }
            json_filename = f"{config}_{idx:06d}_system.json"
            json_path = os.path.join(output_path, json_filename)
            with open(json_path, 'w') as json_file:
                json.dump(json_data, json_file, indent=2) 
