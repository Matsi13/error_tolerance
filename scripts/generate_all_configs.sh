#!/bin/bash

# Generate all astra-sim configuration files from a wafer file
# Usage: ./generate_all_configs.sh <wafer_file> [output_base_dir]
# Example:
#   ./generate_all_configs.sh ./output/0070/0070_Wafer.txt ./output/0070

set -e

if [ $# -lt 1 ]; then
    echo "Usage: $0 <wafer_file> [output_base_dir]"
    echo ""
    echo "Example:"
    echo "  $0 ./output/0070/0070_Wafer.txt ./output/0070"
    exit 1
fi

WAFER_FILE="$1"
OUTPUT_BASE="${2:-.}"

CONFIG=$(basename "$WAFER_FILE" | cut -d'_' -f1)

if [ ! -f "$WAFER_FILE" ]; then
    echo "Error: Wafer file not found: $WAFER_FILE"
    exit 1
fi

echo "=== Generating Astra-sim Configuration Files ==="
echo "Wafer file : $WAFER_FILE"
echo "Config     : $CONFIG"
echo "Output base: $OUTPUT_BASE"
echo ""

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PY_DIR="$(cd "$SCRIPT_DIR/../pyscript" && pwd)"

LOGICAL_NETWORK_DIR="$OUTPUT_BASE/logical_network"
NETWORK_DIR="$OUTPUT_BASE/network"
SYSTEM_DIR="$OUTPUT_BASE/system"
CONFIG_DIR="$OUTPUT_BASE/config"
PHYSICAL_NETWORK_DIR="/home/matsi/workspace/astra-sim/configs/$CONFIG/physical_network"

mkdir -p "$LOGICAL_NETWORK_DIR"
mkdir -p "$NETWORK_DIR"
mkdir -p "$SYSTEM_DIR"
mkdir -p "$CONFIG_DIR"

echo "Generating logical_network.json files..."
python3 -c "
import sys
sys.path.insert(0, '$PY_DIR')
from gen_logical_network import generate_logical_network_json_from_wafer_file
generate_logical_network_json_from_wafer_file('$WAFER_FILE', '$LOGICAL_NETWORK_DIR')
"

echo ""
echo "Generating network.yml files..."
python3 -c "
import sys
sys.path.insert(0, '$PY_DIR')
from gen_network import generate_network_yml_from_wafer_file
generate_network_yml_from_wafer_file('$WAFER_FILE', '$NETWORK_DIR')
"

echo ""
echo "Generating system.json files..."
python3 -c "
import sys
sys.path.insert(0, '$PY_DIR')
from gen_system import generate_system_json_from_wafer_file
generate_system_json_from_wafer_file('$WAFER_FILE', '$SYSTEM_DIR')
"

echo ""
echo "Generating config.txt files..."
python3 -c "
import sys
sys.path.insert(0, '$PY_DIR')
from gen_config_txt import generate_config_txt_from_wafer_file
generate_config_txt_from_wafer_file('$WAFER_FILE', '$PHYSICAL_NETWORK_DIR', '$CONFIG_DIR')
"

echo ""
echo "=== Generation Complete ==="
echo "Output directories:"
echo "  Logical network: $LOGICAL_NETWORK_DIR"
echo "  Network config : $NETWORK_DIR"
echo "  System config  : $SYSTEM_DIR"
echo "  Config files   : $CONFIG_DIR"

