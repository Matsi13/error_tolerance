close all; clear; clc

% Macro: Change this variable to switch between different datasets
file_prefix = '0034';  % Change this to '0003', '0034', etc.

% Load files using the macro
eval(['load ' file_prefix '_wafer.txt']);
eval(['load ' file_prefix '_error_curve.txt']);
eval(['num_wafers = size(X' file_prefix '_wafer,1);']);
eval(['datapoints = X' file_prefix '_error_curve;']);

TFLOPs_error = datapoints(:, 1);
num = datapoints(:, 2);
num_designs = num_wafers / num(1);

num = num./num(1);
y_data = 1 - (num-1)/num_designs;
x_data = TFLOPs_error;

% Scale data to fill the fixed axis ranges
x_data_scaled = (x_data - min(x_data)) / (max(x_data) - min(x_data)) * (1.00 - 0.84) + 0.84;
y_data_scaled = (y_data - min(y_data)) / (max(y_data) - min(y_data)) * (1.0 - 0.8) + 0.8;

plot(x_data_scaled, y_data_scaled, 'LineWidth', 2);

% Set fixed axis limits and ticks
%xticks(0.84:0.04:1.00);
xticks([]);
xlim([0.84, 1.00]);
%yticks(0.8:0.02:1);
yticks([]);
ylim([0.8, 1]);


% Remove tick labels to show only tick marks
set(gca, 'XTickLabel', []);
set(gca, 'YTickLabel', []);

