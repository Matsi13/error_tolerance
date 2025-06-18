close all; clear; clc
load ../out/output/0044_error_curve.txt
TFLOPs_error = X0044_error_curve(:, 1);
num = X0044_error_curve(:, 2);

plot(TFLOPs_error, num);
% plot3(TFLOPs_error, paramsize_error, num);
