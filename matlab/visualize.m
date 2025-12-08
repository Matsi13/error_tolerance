close all; clear; clc

% Macro: Change this variable to switch between different datasets
file_prefix = '1108';  % Change this to '0002', '0003', etc.

% Load file using the macro
eval(['load ' file_prefix '_wafer.txt']);
eval(['Generate_output = X' file_prefix '_wafer;']);

TFLOPS = Generate_output(:, 1);
SRAM_capacity = Generate_output(:, 2);
DRAM_capacity = Generate_output(:, 3);
memory_bandwidth = Generate_output(:, 4);
communication_bandwidth = Generate_output(:, 5);

% Scale data to fill the fixed axis ranges
TFLOPS_scaled = (TFLOPS - min(TFLOPS)) / (max(TFLOPS) - min(TFLOPS)) * 1500;
DRAM_capacity_scaled = (DRAM_capacity - min(DRAM_capacity)) / (max(DRAM_capacity) - min(DRAM_capacity)) * 3000;
memory_bandwidth_scaled = (memory_bandwidth - min(memory_bandwidth)) / (max(memory_bandwidth) - min(memory_bandwidth)) * 9000;
communication_bandwidth_scaled = (communication_bandwidth - min(communication_bandwidth)) / (max(communication_bandwidth) - min(communication_bandwidth)) * 8000;

figure1 = scatter3(TFLOPS_scaled, DRAM_capacity_scaled, memory_bandwidth_scaled, 50, communication_bandwidth_scaled);

view([137,5]);
c1 = colorbar;

% Set fixed axis limits and ticks (4 ticks for x, 3 for y, 5 for z, 8 for colorbar)
% These limits are fixed and don't depend on the data
xticks(linspace(0, 1500, 4));
xlim([0, 1500]);
yticks(linspace(0, 3000, 3));
ylim([0, 3000]);
zticks(linspace(0, 9000, 5));
zlim([0, 9000]);
clim([0, 8000]);

% Remove tick labels to show only tick marks
set(gca, 'XTickLabel', []);
set(gca, 'YTickLabel', []);
set(gca, 'ZTickLabel', []);
set(c1, 'TickLabels', []);






