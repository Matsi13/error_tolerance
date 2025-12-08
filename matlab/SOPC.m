close; clear; clc
rng(13);

perf_oracle = 0.6 + 0.4 * rand([20,1]);

err_mid = 0.08;
err_low = 0.15;

perf_mid = perf_oracle .* (1 + err_mid * (rand([20,1])) .* (2 * randi(2,[20,1]) - 3));
perf_mid_ceil = perf_oracle * (1 + err_mid);
perf_mid_floor = perf_oracle * (1 - err_mid);

perf_mid_triple = [perf_mid_floor; perf_oracle - perf_mid_floor; perf_mid_ceil - perf_oracle];
perf_mid_double = [perf_mid_floor, perf_mid_ceil - perf_mid_floor];

perf_low = perf_oracle .* (1 + err_low * (rand([20,1])) .* (2 * randi(2,[20,1]) - 3));
perf_low_ceil = perf_oracle * (1 + err_low);
perf_low_floor = perf_oracle * (1 - err_low);
perf_low_triple = [perf_low_floor; perf_oracle - perf_low_floor; perf_low_ceil - perf_oracle];
perf_low_double = [perf_low_floor, perf_low_ceil - perf_low_floor];

perf_mid_predict_floor = perf_mid / (1 + err_mid);
perf_mid_predict_ceil = perf_mid / (1 - err_mid);
perf_mid_predict_double = [perf_mid_predict_floor, perf_mid_predict_ceil - perf_mid_predict_floor];

perf_low_predict_floor = perf_low / (1 + err_low);
perf_low_predict_ceil = perf_low / (1 - err_low);
perf_low_predict_double = [perf_low_predict_floor, perf_low_predict_ceil - perf_low_predict_floor];


x_ordinate = (1:20)';
% figure(1);
% bar(perf_oracle);
% ylim([0 1.2]);

% figure(2);
% bar(perf_mid_double,'stacked');
% ylim([0.4 1.2]);
% hold on;
% scatter(x_ordinate, perf_mid, 'filled');
% scatter(x_ordinate, perf_oracle,'filled');
% hold off;
% 
% 
% figure(3);
% bar(perf_low_double,'stacked');
% ylim([0.4 1.2]);
% hold on;
% scatter(x_ordinate, perf_low, 'filled');
% scatter(x_ordinate, perf_oracle,'filled');
% hold off;

% figure(3);
% bar(perf_low_double,'stacked');
% ylim([0.4 1.2]);
% hold on;
% scatter(perf_oracle', perf_low', 'filled');
% hold off;

figure(4);
bar(perf_mid_predict_double,'stacked');
ylim([0.4 1.2]);
hold on;
scatter(x_ordinate, perf_mid, 'filled');
hold off;

figure(5);
bar(perf_low_predict_double,'stacked');
ylim([0.4 1.2]);
hold on;
scatter(x_ordinate, perf_low, 'filled');
hold off;

figure(6);
scatter(x_ordinate, perf_oracle, 'filled');
ylim([0.4 1.2]);