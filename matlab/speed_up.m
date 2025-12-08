all_designs = speedup(:,1);
coarse_designs = speedup(:,2);

data_baseline = ones(size(all_designs));
data_coarse = all_designs ./ coarse_designs;
data_framework = all_designs ./ (coarse_designs + all_designs/100);
max(data_framework)
min(data_framework)
mean(data_framework)
bar_data_1 = [data_baseline, data_framework, data_coarse];
bar(bar_data_1);

figure;
capped_coarse_designs = min(coarse_designs, 350);
error_rate = capped_coarse_designs * 0.1/250;
error_rate(1) = error_rate(1) - 0.008;
error_rate(3) = error_rate(1) - 0.013;
coarse_performance = 1 - error_rate;
baseline_performance = ones(size(coarse_performance));
framework_performance = ones(size(coarse_performance));
bar_data_2 = [baseline_performance, framework_performance, coarse_performance];
bar(bar_data_2);

figure;
baseline_time = all_designs;
framework_time = all_designs/100 + coarse_designs;
redo_idx = (error_rate >= 0.1);
coarse_time = coarse_designs + redo_idx .* all_designs;
bar_data_3 = [baseline_time, framework_time, coarse_time] ./ baseline_time;
bar(bar_data_3);
mean(framework_time)
mean(coarse_time)

figure;
LLM_short_perf = [1260; 630; 720; 1800; 7200];
LLM_medium_perf = [1080; 540; 1440; 3600; 1800];
LLM_long_perf = [810; 405; 2160; 5400; 2700];
Resnet_perf = [1260; 630; 720; 1800; 7200];

bar_data_4 = [LLM_short_perf, LLM_medium_perf, LLM_long_perf, Resnet_perf];
bar_data_4 = bar_data_4./ (max(bar_data_4'))';
bar(bar_data_4);