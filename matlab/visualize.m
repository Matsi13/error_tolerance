close all; clear; clc
load ../out/output/Generate_output.txt
TFLOPS = Generate_output(:, 1);
SRAM_capacity = Generate_output(:, 2);
DRAM_capacity = Generate_output(:, 3);
memory_bandwidth = Generate_output(:, 4);
communication_bandwidth = Generate_output(:, 5);

scatter3(TFLOPS, DRAM_capacity, memory_bandwidth, 50, communication_bandwidth, "filled");
colorbar
