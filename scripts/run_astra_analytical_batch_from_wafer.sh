#!/usr/bin/env bash

# Run Astra-Sim analytical for every wafer entry in a wafer file.
# It auto-detects config id from wafer filename: 0070_Wafer.txt -> 0070
#
# Usage:
#   ./scripts/run_astra_analytical_batch_from_wafer.sh \
#     <wafer_file> <workload_dir> <system_dir> <network_dir> <remote_memory_file> <output_dir>
#
# Example:
#   ./scripts/run_astra_analytical_batch_from_wafer.sh \
#     ./output/0070/0070_Wafer.txt \
#     ./configs/0070/workload_et \
#     ./configs/0070/system \
#     ./configs/0070/network \
#     ./configs/0070/remote_memory.json \
#     ./configs/0070/analytical_output

set -euo pipefail

if [ "$#" -ne 6 ]; then
  echo "Usage: $0 <wafer_file> <workload_dir> <system_dir> <network_dir> <remote_memory_file> <output_dir>"
  exit 1
fi

WAFER_FILE="$1"
WORKLOAD_DIR="$2"
SYSTEM_DIR="$3"
NETWORK_DIR="$4"
REMOTE_MEMORY_FILE="$5"
OUTPUT_DIR="$6"

ASTRA_ANALYTICAL_BIN="${ASTRA_ANALYTICAL_BIN:-./build/astra_analytical/build/bin/AstraSim_Analytical_Congestion_Unaware}"

if [ ! -f "$WAFER_FILE" ]; then
  echo "Error: wafer file not found: $WAFER_FILE"
  exit 2
fi

if [ ! -f "$REMOTE_MEMORY_FILE" ]; then
  echo "Error: remote memory file not found: $REMOTE_MEMORY_FILE"
  exit 2
fi

if [ ! -x "$ASTRA_ANALYTICAL_BIN" ]; then
  echo "Error: Astra analytical binary not found or not executable: $ASTRA_ANALYTICAL_BIN"
  exit 3
fi

mkdir -p "$OUTPUT_DIR"

base_name="$(basename "$WAFER_FILE")"
config="${base_name%%_*}"

wafer_num="$(awk 'NF>0 {c++} END {print c+0}' "$WAFER_FILE")"

echo "=== Astra-Sim Analytical Batch (from wafer file) ==="
echo "Wafer file : $WAFER_FILE"
echo "Config     : $config"
echo "Wafer count: $wafer_num"
echo "Workload dir: $WORKLOAD_DIR"
echo "System dir  : $SYSTEM_DIR"
echo "Network dir : $NETWORK_DIR"
echo "Remote file : $REMOTE_MEMORY_FILE"
echo "Output dir  : $OUTPUT_DIR"
echo

success=0
failed=0

for ((idx=0; idx<wafer_num; idx++)); do
  idx_padded="$(printf "%06d" "$idx")"

  workload_file="$WORKLOAD_DIR/${config}_${idx_padded}_workload"
  system_file="$SYSTEM_DIR/${config}_${idx_padded}_system.json"
  network_file="$NETWORK_DIR/${config}_${idx_padded}_network.yml"
  output_file="$OUTPUT_DIR/${config}_${idx_padded}_analytical_output.txt"

  echo "[Wafer $idx_padded]"

  if [ ! -e "$workload_file" ]; then
    echo "  Missing workload: $workload_file"
    failed=$((failed + 1))
    continue
  fi
  if [ ! -f "$system_file" ]; then
    echo "  Missing system file: $system_file"
    failed=$((failed + 1))
    continue
  fi
  if [ ! -f "$network_file" ]; then
    echo "  Missing network file: $network_file"
    failed=$((failed + 1))
    continue
  fi

  "$ASTRA_ANALYTICAL_BIN" \
    --workload-configuration="$workload_file" \
    --system-configuration="$system_file" \
    --network-configuration="$network_file" \
    --remote-memory-configuration="$REMOTE_MEMORY_FILE" \
    > "$output_file" 2>&1

  echo "  Done -> $output_file"
  success=$((success + 1))
done

echo
echo "=== Batch Complete ==="
echo "Success: $success"
echo "Failed : $failed"

if [ "$failed" -gt 0 ]; then
  exit 4
fi