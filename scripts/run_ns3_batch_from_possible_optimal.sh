#!/usr/bin/env bash

# Run Astra-Sim ns3 backend for wafers listed in possible_optimal.txt.
#
# Usage:
#   ./scripts/run_ns3_batch_from_possible_optimal.sh \
#     <idx_file> <wafer_file> <workload_dir> <system_dir> <network_config_dir> \
#     <remote_memory_file> <logical_topology_dir> <output_dir>
#
# Example:
#   ./scripts/run_ns3_batch_from_possible_optimal.sh \
#     ./output/0070/possible_optimal.txt \
#     ./output/0070/0070_Wafer.txt \
#     ./configs/0070/workload_et \
#     ./configs/0070/system \
#     ./configs/0070/config \
#     ./configs/0070/remote_memory.json \
#     ./configs/0070/logical_network \
#     ./configs/0070/ns3_output

set -euo pipefail

if [ "$#" -ne 8 ]; then
  echo "Usage: $0 <idx_file> <wafer_file> <workload_dir> <system_dir> <network_config_dir> <remote_memory_file> <logical_topology_dir> <output_dir>"
  exit 1
fi

IDX_FILE="$1"
WAFER_FILE="$2"
WORKLOAD_DIR="$3"
SYSTEM_DIR="$4"
NETWORK_CFG_DIR="$5"
REMOTE_MEMORY_FILE="$6"
LOGICAL_DIR="$7"
OUTPUT_DIR="$8"

ASTRA_NS3_BIN="${ASTRA_NS3_BIN:-./extern/network_backend/ns-3/build/scratch/ns3.42-AstraSimNetwork-default}"

if [ ! -f "$IDX_FILE" ]; then
  echo "Error: idx file not found: $IDX_FILE"
  exit 2
fi
if [ ! -f "$WAFER_FILE" ]; then
  echo "Error: wafer file not found: $WAFER_FILE"
  exit 2
fi
if [ ! -f "$REMOTE_MEMORY_FILE" ]; then
  echo "Error: remote memory file not found: $REMOTE_MEMORY_FILE"
  exit 2
fi
if [ ! -x "$ASTRA_NS3_BIN" ]; then
  echo "Error: ns3 binary not found or not executable: $ASTRA_NS3_BIN"
  exit 3
fi

mkdir -p "$OUTPUT_DIR"

base_name="$(basename "$WAFER_FILE")"
config="${base_name%%_*}"

echo "=== Astra-Sim ns3 Batch (possible_optimal) ==="
echo "Idx file     : $IDX_FILE"
echo "Wafer file   : $WAFER_FILE"
echo "Config       : $config"
echo "Workload dir : $WORKLOAD_DIR"
echo "System dir   : $SYSTEM_DIR"
echo "Network dir  : $NETWORK_CFG_DIR"
echo "Remote file  : $REMOTE_MEMORY_FILE"
echo "Logical dir  : $LOGICAL_DIR"
echo "Output dir   : $OUTPUT_DIR"
echo

success=0
failed=0

while IFS= read -r line || [ -n "$line" ]; do
  idx_raw="$(echo "$line" | tr -d '[:space:]')"
  if [ -z "$idx_raw" ]; then
    continue
  fi
  if ! [[ "$idx_raw" =~ ^[0-9]+$ ]]; then
    echo "Skip invalid idx line: '$line'"
    continue
  fi

  idx_padded="$(printf "%06d" "$idx_raw")"

  workload_file="$WORKLOAD_DIR/${config}_${idx_padded}_workload"
  system_file="$SYSTEM_DIR/${config}_${idx_padded}_system.json"
  network_file="$NETWORK_CFG_DIR/${config}_${idx_padded}_config.txt"
  logical_file="$LOGICAL_DIR/${config}_${idx_padded}_logical_network.json"
  output_file="$OUTPUT_DIR/${config}_${idx_padded}_ns3_output.txt"

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
  if [ ! -f "$logical_file" ]; then
    echo "  Missing logical file: $logical_file"
    failed=$((failed + 1))
    continue
  fi

  "$ASTRA_NS3_BIN" \
    --workload-configuration="$workload_file" \
    --system-configuration="$system_file" \
    --network-configuration="$network_file" \
    --remote-memory-configuration="$REMOTE_MEMORY_FILE" \
    --logical-topology-configuration="$logical_file" \
    --comm-group-configuration="empty" \
    > "$output_file" 2>&1

  echo "  Done -> $output_file"
  success=$((success + 1))
done < "$IDX_FILE"

echo
echo "=== ns3 Batch Complete ==="
echo "Success: $success"
echo "Failed : $failed"

if [ "$failed" -gt 0 ]; then
  exit 4
fi