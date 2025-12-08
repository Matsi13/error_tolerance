close all; clear; clc
load 0034_stair_curve.txt
TFLOPs_error = X0034_stair_curve(:, 1);
num = X0034_stair_curve(:, 2);
num = num./num(1);
x = 2 * (TFLOPs_error(1:50));
y = num(1:50)/max(num);

figure(1);
plot(x, y, 'LineWidth', 2);

hold on;
scatter(x, y, ".", "black");
% plot3(TFLOPs_error, paramsize_error, num);

