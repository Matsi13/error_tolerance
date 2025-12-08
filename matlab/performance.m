close all; clear; clc
load 0010_wafer.txt
Generate_output = X0010_wafer;
TFLOPS = Generate_output(:, 1);
SRAM_capacity = Generate_output(:, 2);
DRAM_capacity = Generate_output(:, 3);
memory_bandwidth = Generate_output(:, 4);
communication_bandwidth = Generate_output(:, 5);
performance_data = TFLOPS./max(TFLOPS);
[temp1, idx, temp2] = unique(performance_data);
idx = unique(idx);
bar(performance_data(idx))
% plot(performance_data(idx),'-o','MarkerSize',5)
% plot(performance_data(idx),'o','MarkerSize',10,...
%     'MarkerEdgeColor','red',...
%     'MarkerFaceColor',[1 .6 .6])