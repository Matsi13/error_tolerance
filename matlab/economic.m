all_designs = speedup(:,1);
coarse_designs = speedup(:,2);

% data_baseline = ones(size(all_designs));
% data_coarse = all_designs ./ coarse_designs;
% data_framework = all_designs ./ (coarse_designs + all_designs/100);
% max(data_framework)
% min(data_framework)
% mean(data_framework)
% bar_data_1 = [data_baseline, data_framework, data_coarse];
% bar(bar_data_1);
% 
% figure;
% capped_coarse_designs = min(coarse_designs, 350);
% error_rate = capped_coarse_designs * 0.1/250;
% error_rate(1) = error_rate(1) - 0.008;
% error_rate(3) = error_rate(1) - 0.013;
% coarse_performance = 1 - error_rate;
% baseline_performance = ones(size(coarse_performance));
% framework_performance = ones(size(coarse_performance));
% bar_data_2 = [baseline_performance, framework_performance, coarse_performance];
% bar(bar_data_2);

figure;
redo_idx = (error_rate >= 0.1);
framework_search_time = all_designs/100 + coarse_designs;

coarse_search_time = all_designs/100 + redo_idx .* all_designs/10;
bar_data_3 = 0.3 * [framework_search_time, coarse_search_time]./framework_time;

framework_RTL_time = ones(size(framework_time));
coarse_RTL_time = 1 + redo_idx .* ones(size(framework_time));
bar_data_4 = 0.7 * [framework_RTL_time, coarse_RTL_time];

bar_data_5 = bar_data_3 + bar_data_4;
bar(bar_data_5);

% bar(bar_data_3);
% mean(framework_time)
% mean(coarse_time)
% figure;
% bar_data_4 = [mean(framework_time), mean(coarse_time)];
% bar(bar_data_4);
% 
% figure;

% 
% framework_total_time = framework_time + framework_RTL_time;
% coarse_total_time = corase_time + coarse_RTL_time;


