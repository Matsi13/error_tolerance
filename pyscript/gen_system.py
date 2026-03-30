import os
import json
from wafer_utils import extract_config, load_wafer_file, ensure_dir, out_filename


def generate_system_json_from_wafer_file(wafer_file: str, output_path: str):
    config = extract_config(wafer_file)
    ensure_dir(output_path)
    for entry in load_wafer_file(wafer_file):
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
            "local_mem_bw": entry.mem_bw,
            "boost_mode": 0
        }
        filename = out_filename(config, entry.idx, "system.json")
        with open(os.path.join(output_path, filename), "w") as f:
            json.dump(json_data, f, indent=2)
        print(f"Generated: {filename}")


def generate_network_json(config: str, input_path: str, output_path: str):
    """Backward-compatible wrapper."""
    wafer_file = os.path.join(input_path, f"{config}_Wafer.txt")
    generate_system_json_from_wafer_file(wafer_file, output_path)


if __name__ == "__main__":
    wafer_file = "/data/login_home/lijinxi/error_tolerance/output/0070/0070_Wafer.txt"
    output_path = "/data/login_home/lijinxi/error_tolerance/output/system"
    generate_system_json_from_wafer_file(wafer_file=wafer_file, output_path=output_path)
