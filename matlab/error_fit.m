close all; clear; clc
load ../out/output/error_curve.txt
TFLOPs_error = error_curve(:, 1);
paramsize_error = error_curve(:, 2);
num = error_curve(:, 3);

plot3(TFLOPs_error, paramsize_error, num);
