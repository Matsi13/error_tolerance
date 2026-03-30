#!/usr/bin/env python3

import os
import sys
import subprocess
from pathlib import Path


class WaferConfig:
    def __init__(self, rows, columns, wafer_num):
        self.rows = rows
        self.columns = columns
        self.num_npus = rows * columns
        self.wafer_num = wafer_num


def _is_int_token(token):
    try:
        int(token)
        return True
    except Exception:
        return False


def extract_config_from_wafer_filename(wafer_config_file):
    """Extract config ID from wafer filename.
    Example: ./output/0070/0070_Wafer.txt -> 0070
    """
    base = os.path.basename(wafer_config_file)
    stem, _ = os.path.splitext(base)

    # Prefer prefix before underscore, e.g., 0070_Wafer
    if '_' in stem:
        return stem.split('_')[0]

    # Fallback: use parent directory name if numeric-like
    parent = os.path.basename(os.path.dirname(wafer_config_file))
    if parent:
        return parent

    return stem


def parse_wafer_line(line, line_idx):
    """Parse a wafer line.

    Supports two formats:
    1) Original format (no wafer_num):
       [TFLOPS SRAM DRAM mem_bw comm_bw rows cols ...]
       -> rows at idx 5, cols at idx 6, wafer_num = line_idx + 1

    2) Extended format (wafer_num + config at head):
       [wafer_num config TFLOPS SRAM DRAM mem_bw comm_bw rows cols ...]
       -> rows at idx 7, cols at idx 8, wafer_num from file
    """
    parts = line.strip().split()
    if len(parts) < 7:
        raise ValueError('Invalid wafer line format: ' + line)

    # Try extended format first if first token looks like wafer_num
    if len(parts) >= 9 and _is_int_token(parts[0]):
        try:
            wafer_num = int(parts[0])
            rows = int(parts[7])
            columns = int(parts[8])
            return WaferConfig(rows, columns, wafer_num)
        except Exception:
            pass

    # Fallback to original format
    try:
        rows = int(parts[5])
        columns = int(parts[6])
        wafer_num = line_idx + 1
        return WaferConfig(rows, columns, wafer_num)
    except Exception:
        raise ValueError('Unable to parse rows/columns from line: ' + line)


def run_chakra_converter(input_workload, output_base_dir, wafer, config):
    """Execute chakra_converter for one wafer.

    Output folder:
      <output_base_dir>/<wafer_num:04d>/
    Output prefix:
      <config>_<wafer_num:04d>_workload
    """
    wafer_output_dir = os.path.join(output_base_dir, '{:04d}'.format(wafer.wafer_num))
    Path(wafer_output_dir).mkdir(parents=True, exist_ok=True)

    output_prefix = os.path.join(
        wafer_output_dir,
        '{}_{:04d}_workload'.format(config, wafer.wafer_num)
    )

    cmd = [
        'chakra_converter', 'Text',
        '--input', input_workload,
        '--output', output_prefix,
        '--num-npus', str(wafer.num_npus),
        '--num-passes', '1'
    ]

    print('  [Wafer {}] Running: {}'.format(wafer.wafer_num, ' '.join(cmd)))
    print('  [Wafer {}] Output dir: {}'.format(wafer.wafer_num, wafer_output_dir))

    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=False)
        if result.returncode != 0:
            print('  [Wafer {}] Error: chakra_converter failed (code {})'.format(
                wafer.wafer_num, result.returncode))
            if result.stderr:
                print('  [Wafer {}] stderr: {}'.format(wafer.wafer_num, result.stderr.strip()))
            return False

        print('  [Wafer {}] Success: {} NPUs -> {}'.format(
            wafer.wafer_num, wafer.num_npus, wafer_output_dir))
        return True

    except FileNotFoundError:
        print('  [Wafer {}] Error: chakra_converter not found in PATH'.format(wafer.wafer_num))
        return False
    except Exception as e:
        print('  [Wafer {}] Error: {}'.format(wafer.wafer_num, e))
        return False


def main():
    if len(sys.argv) < 4:
        print('Usage: {} <wafer_config_file> <text_workload_file> <output_directory>'.format(sys.argv[0]))
        print('Example: {} ./output/0070/0070_Wafer.txt ./workloads/Llama_3B.txt ./output/0070/workload_et'.format(sys.argv[0]))
        sys.exit(1)

    wafer_config_file = sys.argv[1]
    text_workload_file = sys.argv[2]
    output_directory = sys.argv[3]

    if not os.path.exists(wafer_config_file):
        print('Error: Wafer configuration file not found: ' + wafer_config_file)
        sys.exit(1)
    if not os.path.exists(text_workload_file):
        print('Error: Text workload file not found: ' + text_workload_file)
        sys.exit(1)

    config = extract_config_from_wafer_filename(wafer_config_file)

    print('=== Chakra Workload Generator ===')
    print('Wafer config file: ' + wafer_config_file)
    print('Detected config : ' + config)
    print('Text workload   : ' + text_workload_file)
    print('Output base     : ' + output_directory)
    print('')

    wafers = []
    print('Reading wafer configurations...')
    try:
        with open(wafer_config_file, 'r') as f:
            for idx, line in enumerate(f):
                line = line.strip()
                if not line:
                    continue
                try:
                    wafer = parse_wafer_line(line, idx)
                    wafers.append(wafer)
                    print('  Wafer {:04d}: {}x{} = {} NPUs'.format(
                        wafer.wafer_num, wafer.rows, wafer.columns, wafer.num_npus))
                except ValueError as e:
                    print('  Warning: skipping line {}: {}'.format(idx, e))
    except IOError as e:
        print('Error: {}'.format(e))
        sys.exit(1)

    if not wafers:
        print('Error: No valid wafer configurations found')
        sys.exit(1)

    print('\nTotal wafers: {}\n'.format(len(wafers)))

    print('Generating Chakra workloads...')
    success_count = 0
    fail_count = 0

    for wafer in wafers:
        if run_chakra_converter(text_workload_file, output_directory, wafer, config):
            success_count += 1
        else:
            fail_count += 1

    print('\n=== Generation Complete ===')
    print('Successful : {}'.format(success_count))
    print('Failed     : {}'.format(fail_count))
    print('Output base: ' + output_directory)

    sys.exit(1 if fail_count > 0 else 0)


if __name__ == '__main__':
    main()
