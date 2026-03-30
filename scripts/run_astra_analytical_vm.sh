#!/usr/bin/env bash

# Run one Astra-Sim analytical simulation on VM.
#
# Usage:
#   ./scripts/run_astra_analytical_vm.sh \
#     <workload_config> <system_config> <network_config> <remote_memory_config> [output_file]
#
# Example:
#   ./scripts/run_astra_analytical_vm.sh \
#     ./configs/config2/workload_et/Resnet50_DataParallel \
#     ./configs/config2/system.json \
#     ./configs/config2/network.yml \
#     ./configs/config2/remote_memory.json \
#     ./configs/config2/analytical_output.txt

set -euo pipefail

if [ "$#" -lt 4 ] || [ "$#" -gt 5 ]; then
  echo "Usage: $0 <workload_config> <system_config> <network_config> <remote_memory_config> [output_file]"
  exit 1
fi

WORKLOAD_CFG="$1"
SYSTEM_CFG="$2"
NETWORK_CFG="$3"
REMOTE_MEM_CFG="$4"
OUTPUT_FILE="${5:-}"

# You can override binary path via environment variable:
#   ASTRA_ANALYTICAL_BIN=/path/to/AstraSim_Analytical_Congestion_Unaware ./scripts/run_astra_analytical_vm.sh ...
ASTRA_ANALYTICAL_BIN="${ASTRA_ANALYTICAL_BIN:-./build/astra_analytical/build/bin/AstraSim_Analytical_Congestion_Unaware}"

for f in "$WORKLOAD_CFG" "$SYSTEM_CFG" "$NETWORK_CFG" "$REMOTE_MEM_CFG"; do
  if [ ! -e "$f" ]; then
    echo "Error: file not found -> $f"
    exit 2
  fi
done

if [ ! -x "$ASTRA_ANALYTICAL_BIN" ]; then
  echo "Error: Astra analytical binary not found or not executable: $ASTRA_ANALYTICAL_BIN"
  exit 3
fi

echo "Running Astra-Sim analytical simulation..."
echo "  Binary  : $ASTRA_ANALYTICAL_BIN"
echo "  Workload: $WORKLOAD_CFG"
echo "  System  : $SYSTEM_CFG"
echo "  Network : $NETWORK_CFG"
echo "  Remote  : $REMOTE_MEM_CFG"

CMD=(
  "$ASTRA_ANALYTICAL_BIN"
  "--workload-configuration=$WORKLOAD_CFG"
  "--system-configuration=$SYSTEM_CFG"
  "--network-configuration=$NETWORK_CFG"
  "--remote-memory-configuration=$REMOTE_MEM_CFG"
)

if [ -n "$OUTPUT_FILE" ]; then
  mkdir -p "$(dirname "$OUTPUT_FILE")"
  "${CMD[@]}" > "$OUTPUT_FILE" 2>&1
  echo "Done. Output saved to: $OUTPUT_FILE"
else
  "${CMD[@]}"
  echo "Done."
fi
