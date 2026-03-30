import os
from wafer_utils import extract_config, load_wafer_file, ensure_dir, out_filename


def generate_network_yml_from_wafer_file(wafer_file: str, output_path: str):
    config = extract_config(wafer_file)
    ensure_dir(output_path)
    for entry in load_wafer_file(wafer_file):
        bw = entry.comm_bw / 4.0
        yml_content = (
            "topology: [Ring, Ring]\n"
            f"npus_count: [{entry.columns}, {entry.rows}]\n"
            f"bandwidth: [{bw}, {bw}]\n"
            "latency: [10.0, 10.0]\n"
        )
        filename = out_filename(config, entry.idx, "network.yml")
        with open(os.path.join(output_path, filename), "w") as f:
            f.write(yml_content)
        print(f"Generated: {filename}")


def generate_network_yml(config: str, input_path: str, output_path: str):
    """Backward-compatible wrapper."""
    wafer_file = os.path.join(input_path, f"{config}_Wafer.txt")
    generate_network_yml_from_wafer_file(wafer_file, output_path)


if __name__ == "__main__":
    wafer_file = "/data/login_home/lijinxi/error_tolerance/output/0070/0070_Wafer.txt"
    output_path = "/data/login_home/lijinxi/error_tolerance/output/network"
    generate_network_yml_from_wafer_file(wafer_file=wafer_file, output_path=output_path)
