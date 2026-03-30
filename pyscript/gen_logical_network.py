import os
import json
from wafer_utils import extract_config, load_wafer_file, ensure_dir, out_filename


def generate_logical_network_json_from_wafer_file(wafer_file: str, output_path: str):
    config = extract_config(wafer_file)
    ensure_dir(output_path)
    for entry in load_wafer_file(wafer_file):
        json_data = {"logical-dims": [str(entry.columns), str(entry.rows)]}
        filename = out_filename(config, entry.idx, "logical_network.json")
        with open(os.path.join(output_path, filename), "w") as f:
            json.dump(json_data, f, indent=2)
        print(f"Generated: {filename}")


def generate_logical_network_json(config: str, input_path: str, output_path: str):
    """Backward-compatible wrapper."""
    wafer_file = os.path.join(input_path, f"{config}_Wafer.txt")
    generate_logical_network_json_from_wafer_file(wafer_file, output_path)


if __name__ == "__main__":
    wafer_file = "/data/login_home/lijinxi/error_tolerance/output/0070/0070_Wafer.txt"
    output_path = "/data/login_home/lijinxi/error_tolerance/output/logical_network"
    generate_logical_network_json_from_wafer_file(wafer_file=wafer_file, output_path=output_path)
