#!/bin/bash

# Directly specify config and wafer_num values
config="0060"
wafer_num=17

echo "Using config: $config, wafer_num: $wafer_num"

echo "Changing to chakra directory..."
cd /data/login_home/lijinxi/astra-sim/extern/graph_frontend/chakra/

# Read wafer file and extract die numbers
wafer_file="/data/login_home/lijinxi/error_tolerance/output/wafer/${config}_wafer.txt"

if [ ! -f "$wafer_file" ]; then
    echo "Error: Wafer file $wafer_file not found!"
    exit 1
fi

echo "Reading wafer file: $wafer_file"

# Use Python to extract die numbers (rows * columns from columns 6 and 7)
die_numbers=$(python3 -c "
import sys
die_nums = []
with open('$wafer_file', 'r') as f:
    for i, line in enumerate(f):
        if i >= $wafer_num:
            break
        parts = line.strip().split()
        if len(parts) >= 7:
            rows = int(parts[5])  # 6th column (0-indexed)
            cols = int(parts[6])  # 7th column (0-indexed)
            die_num = rows * cols
            die_nums.append(die_num)
        else:
            print(f'Warning: Line {i+1} has insufficient columns', file=sys.stderr)
            die_nums.append(1)  # Default value
print(' '.join(map(str, die_nums)))
")

# Convert die_numbers string to array
die_array=($die_numbers)

echo "Extracted die numbers: ${die_array[@]}"

# Check if we have the correct number of die numbers
if [ ${#die_array[@]} -ne $wafer_num ]; then
    echo "Error: Expected $wafer_num die numbers, but got ${#die_array[@]}"
    exit 1
fi

# Create output directory if it doesn't exist
output_dir="/data/login_home/lijinxi/error_tolerance/output/workload_et"
mkdir -p "$output_dir"

# Run chakra_converter for each wafer
for ((idx=0; idx<=wafer_num-1; idx++)); do
    die_num=${die_array[$((idx-1))]}
    printf "Processing wafer %06d with die_num=%d\n" $idx $die_num
    
    input_file="/data/login_home/lijinxi/error_tolerance/output/workload/${config}_$(printf "%06d" $idx)_workload.txt"
    output_file="/data/login_home/lijinxi/error_tolerance/output/workload_et/${config}_$(printf "%06d" $idx)_workload"
    
    # Check if input file exists
    if [ ! -f "$input_file" ]; then
        echo "Warning: Input file $input_file not found, skipping..."
        continue
    fi
    
    echo "Running chakra_converter for wafer $idx..."
    chakra_converter Text \
        --input "$input_file" \
        --output "$output_file" \
        --num-npus "$die_num" \
        --num-passes 1
    
    if [ $? -eq 0 ]; then
        echo "Successfully processed wafer $idx"
    else
        echo "Error processing wafer $idx"
    fi
done

echo "Batch processing completed!" 