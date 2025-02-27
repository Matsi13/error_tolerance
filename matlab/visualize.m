close all; clear; clc
load ../out/output/wafer.txt
TFLOPS = wafer(:, 1);
capacity = wafer(:, 2);
memory_bandwidth = wafer(:, 3);
communication_bandwidth = wafer(:, 4);

scatter3(TFLOPS, capacity, communication_bandwidth, 50, memory_bandwidth, "filled");
colorbar
