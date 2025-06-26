import os
import json
def generate_l0gical_network_json(config: str, input_path: str, output_path: str):
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
                "active-chunks-per-dimnesion": 1,
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
