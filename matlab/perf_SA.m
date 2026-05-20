close all; clear; clc

% Read performance table from Excel
% Correct layout (row-wise case studies):
%   - Row 1: captions (A1: case label header, B1:E1: method names)
%   - Column A: case-study captions (rows 2:13)
%   - Numeric data: B2:E13 (12 case studies x 4 methods)
%     Col 2: fine-grained search (optimal wafer)
%     Col 3: our method
%     Col 4: simulated annealing
%     Col 5: coarse-grained search

% Primary path requested by user + fallback when run from repo root
path_candidates = {
    '../output/paper/experiment/matlab/performance.xlsx', ...
    './output/paper/experiment/matlab/performance.xlsx'
};

xlsx_path = '';
for i = 1:numel(path_candidates)
    if isfile(path_candidates{i})
        xlsx_path = path_candidates{i};
        break;
    end
end

if isempty(xlsx_path)
    error('Cannot find performance.xlsx. Expected at ../output/paper/experiment/matlab/performance.xlsx');
end

raw = readcell(xlsx_path);

if size(raw,1) < 13 || size(raw,2) < 5
    error('performance.xlsx format mismatch: expected at least 13 rows and 5 columns.');
end

% Captions
case_labels = raw(2:13, 1);   % 12 case studies in first column
method_labels = raw(1, 2:5);  % 4 methods in first row

% Numeric data block: 12 x 4
perf_data = nan(12, 4);
for r = 1:12
    for c = 1:4
        v = raw{r+1, c+1};
        if isnumeric(v)
            perf_data(r,c) = v;
        elseif ischar(v) || isstring(v)
            perf_data(r,c) = str2double(v);
        end
    end
end

if any(isnan(perf_data), 'all')
    error('performance.xlsx contains non-numeric or missing values in B2:E13.');
end

% Figure 1: grouped bar chart for 12 case studies
figure(1);
bar(perf_data);
grid on;
legend(method_labels, 'Location', 'best');
set(gca, 'XTick', 1:12, 'XTickLabel', case_labels);
xtickangle(30);
xlabel('Case Studies');
ylabel('Performance');
title('Performance Comparison Across 12 Case Studies');

% Figure 2: normalize by fine-grained search (column 1)
baseline = perf_data(:,1);
relative_data = perf_data ./ baseline;

figure(2);
bar(relative_data);
grid on;
legend(method_labels, 'Location', 'best');
set(gca, 'XTick', 1:12, 'XTickLabel', case_labels);
xtickangle(30);
xlabel('Case Studies');
ylabel('Relative Performance (vs Fine-Grained)');
title('Relative Performance (Fine-Grained = 1.0)');
ylim([0.8, 1.0]);

% Print summary
fprintf('Loaded file: %s\n', xlsx_path);
for c = 1:4
    fprintf('%s: min=%.4f, mean=%.4f, max=%.4f\n', string(method_labels{c}), ...
        min(perf_data(:,c)), mean(perf_data(:,c)), max(perf_data(:,c)));
end
