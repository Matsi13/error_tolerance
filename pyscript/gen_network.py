import os

def generate_network_yml(config: str, input_path: str, output_path: str):
    """
    读取input_path下名为config_wafer.txt的文件，按行生成yml文件到output_path。
    yml文件命名为<config>_<行号（六位整数）>_network.yml。
    yml内容格式见函数说明。
    """
    input_file = os.path.join(input_path, f"{config}_wafer.txt")
    if not os.path.exists(output_path):
        os.makedirs(output_path)
    with open(input_file, 'r') as f:
        for idx, line in enumerate(f, 1):
            parts = line.strip().split()
            if len(parts) < 32:
                continue  # 跳过格式不对的行
            rows = int(parts[5])
            columns = int(parts[6])
            mem_bandwidth = float(parts[30])
            communication_bandwidth = float(parts[31]) / 4
            yml_content = (
                "topology: [Ring, Ring]\n"
                f"npus_count: [{columns}, {rows}]\n"
                f"bandwidth: [{communication_bandwidth}, {communication_bandwidth}]\n"
                "latency: [10.0, 10.0]\n"
            )
            yml_filename = f"{config}_{idx:06d}_network.yml"
            yml_path = os.path.join(output_path, yml_filename)
            with open(yml_path, 'w') as yml_file:
                yml_file.write(yml_content) 

if __name__ == '__main__':

    input_path = "/data/login_home/lijinxi/error_tolerance/output/wafer"
    config = "0060"
    output_path = "/data/login_home/lijinxi/error_tolerance/output/network"
    generate_network_yml(config=config, input_path=input_path, output_path=output_path)