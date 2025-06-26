import os
import json

def generate_logical_network_json(config: str, input_path: str, output_path: str):
    """
    读取input_path下名为config_wafer.txt的文件，按行生成json文件到output_path。
    json文件命名为<config>_<行号（六位整数）>_logical.json。
    只包含logical-dims: [rows, columns]，均为字符串。
    """
    input_file = os.path.join(input_path, f"{config}_wafer.txt")
    if not os.path.exists(output_path):
        os.makedirs(output_path)
    with open(input_file, 'r') as f:
        for idx, line in enumerate(f, 1):
            parts = line.strip().split()
            if len(parts) < 7:
                continue  # 跳过格式不对的行
            rows = str(parts[5])
            columns = str(parts[6])
            json_data = {
                "logical-dims": [rows, columns]
            }
            json_filename = f"{config}_{idx:06d}_logical.json"
            json_path = os.path.join(output_path, json_filename)
            with open(json_path, 'w') as json_file:
                json.dump(json_data, json_file, indent=2) 
