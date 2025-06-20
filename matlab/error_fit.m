close all; clear; clc
load ../out/output/0001_error_curve.txt
TFLOPs_error = X0001_error_curve(:, 1);
num = X0001_error_curve(:, 2);

num = num./num(1);

plot(TFLOPs_error, num);
% plot3(TFLOPs_error, paramsize_error, num);
