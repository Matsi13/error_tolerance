#!/usr/bin/env python3
path = r'd:\VS projects\error_tolerance\pyscript\chakra_workload_generator.py'

new_content = """#!/usr/bin/env python3

import os
import sys
import subprocess
from pathlib import Path


class WaferConfig:
    def __init__(self, rows, columns):
        self.rows = rows
        self.columns = columns
        self.num_npus = rows * columns


def parse_wafer_line(line):
    """Parse a single wafer line and extract rows and columns"""
    parts = line.strip().split()
    if len(parts) < 7:
        raise ValueError("Invalid wafer line format: " + line)
    # Skip first 5 values (TFLOPS, SRAM, DRAM, mem_bw, comm_bw)
    # Read rows (index 5) and columns (index 6)
    rows = int(parts[5])
    columns = int(parts[6])
    return WaferConfig(rows, columns)


def run_chakra_converter(input_workload, output_base_dir, num_npus, wafer_idx):
    """Execute chakra_converter command for a single wafer.
    Each wafer gets its own subdirectory: output_base_dir/XXXX/ (1-indexed)
    """
    # Create wafer-specific output directory: output_base_dir/0001, /0002, ...
    wafer_output_dir = os.path.join(output_base_dir, "{:04d}".format(wafer_idx + 1))
    Path(wafer_output_dir).mkdir(parents=True, exist_ok=True)

    # chakra_converter writes num_npus files named <output>.0.et, <output>.1.et, ...
    output_file = os.path.join(wafer_output_dir, "workload")

    cmd = [
        "chakra_converter", "Text",
        "--input", input_workload,
        "--output", output_file,
        "--num-npus", str(num_npus),
        "--num-passes", "1"
    ]

    cmd_str = " ".join(cmd)
    print("  [Wafer {}] Running: {}".format(wafer_idx, cmd_str))
    print("  [Wafer {}] Output dir: {}".format(wafer_idx, wafer_output_dir))

    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=False)

        if result.returncode != 0:
            print("  [Wafer {}] Error: chakra_converter failed (code {})".format(
                wafer_idx, result.returncode))
            if result.stderr:
                print("  [Wafer {}] stderr: {}".format(wafer_idx, result.stderr.strip()))
            return False

        print("  [Wafer {}] Success: {} NPUs -> {}".format(
            wafer_idx, num_npus, wafer_output_dir))
        return True

    except FileNotFoundError:
        print("  [Wafer {}] Error: chakra_converter not found in PATH".format(wafer_idx))
        return False
    except Exception as e:
        print("  [Wafer {}] Error: {}".format(wafer_idx, e))
        return False


def main():
    if len(sys.argv) < 4:
        print("Usage: {} <wafer_config_file> <text_workload_file> <output_directory>".format(sys.argv[0]))
        print("Example: {} ./output/0070/0070_Wafer.txt ./workloads/Llama_3B.txt ./output/0070/workload_et".format(sys.argv[0]))
        sys.exit(1)

    wafer_config_file = sys.argv[1]
    text_workload_file = sys.argv[2]
    output_directory = sys.argv[3]

    if not os.path.exists(wafer_config_file):
        print("Error: Wafer configuration file not found: " + wafer_config_file)
        sys.exit(1)

    if not os.path.exists(text_workload_file):
        print("Error: Text workload file not found: " + text_workload_file)
        sys.exit(1)

    print("=== Chakra Workload Generator ===")
    print("Wafer config : " + wafer_config_file)
    print("Text workload: " + text_workload_file)
    print("Output base  : " + output_directory)
    print("")

    # Read wafer configurations
    wafers = []
    print("Reading wafer configurations...")
    try:
        with open(wafer_config_file, 'r') as f:
            for idx, line in enumerate(f):
                line = line.strip()
                if not line:
                    continue
                try:
                    config = parse_wafer_line(line)
                    wafers.append(config)
                    print("  Wafer {}: {}x{} = {} NPUs".format(
                        idx, config.rows, config.columns, config.num_npus))
                except ValueError as e:
                    print("  Warning: skipping line {}: {}".format(idx, e))
    except IOError as e:
        print("Error: " + str(e))
        sys.exit(1)

    if not wafers:
        print("Error: No valid wafer configurations found")
        sys.exit(1)

    print("\nTotal wafers: {}\n".format(len(wafers)))

    # Generate Chakra workloads for each wafer
    print("Generating Chakra workloads...")
    success_count = 0
    fail_count = 0

    for i, wafer in enumerate(wafers):
        if run_chakra_converter(text_workload_file, output_directory, wafer.num_npus, i):
            success_count += 1
        else:
            fail_count += 1

    print("\n=== Generation Complete ===")
    print("Successful : " + str(success_count))
    print("Failed     : " + str(fail_count))
    print("Output base: " + output_directory)

    sys.exit(1 if fail_count > 0 else 0)


if __name__ == "__main__":
    main()
"""

with open(path, 'w', encoding='utf-8') as f:
    f.write(new_content)
print('Successfully updated: ' + path)
