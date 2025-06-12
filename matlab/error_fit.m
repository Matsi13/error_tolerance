close all; clear; clc
load ../out/output/0006_error_curve.txt
TFLOPs_error = X0006_error_curve(:, 1);
num = X0006_error_curve(:, 2);

plot(TFLOPs_error, num);
% plot3(TFLOPs_error, paramsize_error, num);
