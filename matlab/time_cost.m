close all; clear; clc

% Define colors for the 10 curves
colors = [
    1 0 0;      % Red
    0 1 0;      % Green
    0 0 1;      % Blue
    1 1 0;      % Yellow
    1 0 1;      % Magenta
    0 1 1;      % Cyan
    0.5 0 0;    % Dark Red
    0 0.5 0;    % Dark Green
    0 0 0.5;    % Dark Blue
    0.5 0.5 0;  % Olive
];

% Define data for each curve (x, y coordinates)
% Each curve can have different number of points
curve_data = {
    % --- Set 1: Long-sequence LLM (Fastest Timeline, Highest Burn Rate) ---
    % Curve 1: WaFoCuS (Slower, more expensive upfront)
    [0, 7.1, 13.3, 19.2; 0, 1.75, 6.11, 15.95];
    
    % Curve 2: Coarse-Grained (Failure + Redo - Now 7 points)
    [0, 5.5, 11.2, 17.0, 22.8, 28.5, 34.3; 0, 1.15, 5.11, 14.61, 15.75, 19.81, 29.45];
    
    % --- Set 2: Medium-sequence LLM ---
    % Curve 3: WaFoCuS (Slower, more expensive upfront)
    [0, 8.2, 15.1, 21.8; 0, 1.39, 4.81, 12.88];
    
    % Curve 4: Coarse-Grained (Failure + Redo - Now 7 points)
    [0, 6.4, 13.0, 19.5, 25.0, 31.8, 38.0; 0, 0.91, 4.11, 11.81, 12.81, 16.23, 23.43];
    
    % --- Set 3: Short-sequence LLM ---
    % Curve 5: WaFoCuS (Slower, more expensive upfront)
    [0, 9.3, 17.2, 24.9; 0, 1.11, 3.81, 10.11];
    
    % Curve 6: Coarse-Grained (Failure + Redo - Now 7 points)
    [0, 7.2, 14.5, 21.9, 28.1, 35.0, 41.5; 0, 0.73, 3.21, 9.41, 10.21, 12.71, 18.31];
    
    % --- Set 4: CNN (Slowest Timeline, Lowest Burn Rate) ---
    % Curve 7: WaFoCuS (Slower, more expensive upfront)
    [0, 10.1, 18.8, 27.1; 0, 0.89, 3.01, 8.21];
    
    % Curve 8: Coarse-Grained (Failure + Redo - Now 7 points)
    [0, 8.0, 16.1, 24.0, 31.8, 39.3, 47.1; 0, 0.61, 2.61, 7.71, 8.35, 10.41, 15.51]
};

% Calculate mean curves
% Curve 9: mean of curves 1, 3, 5, 7 (WaFoCuS methods)
curve9_x = mean([curve_data{1}(1, :); curve_data{3}(1, :); curve_data{5}(1, :); curve_data{7}(1, :)], 1);
curve9_y = mean([curve_data{1}(2, :); curve_data{3}(2, :); curve_data{5}(2, :); curve_data{7}(2, :)], 1);

% Curve 10: mean of curves 2, 4, 6, 8 (Coarse-Grained methods)
curve10_x = mean([curve_data{2}(1, :); curve_data{4}(1, :); curve_data{6}(1, :); curve_data{8}(1, :)], 1);
curve10_y = mean([curve_data{2}(2, :); curve_data{4}(2, :); curve_data{6}(2, :); curve_data{8}(2, :)], 1);

% Add mean curves to curve_data
curve_data{9} = [curve9_x; curve9_y];
curve_data{10} = [curve10_x; curve10_y];
% Create figure for curves
figure(1);
hold on;

% Plot each curve
for i = 1:10
    x_data = curve_data{i}(1, :);
    y_data = curve_data{i}(2, :);
    
    if i <= 8
        % Curves 1-8: lighter, thinner, less transparent
        alpha = 0.7; % Less transparency
        lighter_color = colors(i, :) * 0.9 + 0.1; % Make color lighter
        
        % Plot each part of the curve
        for j = 1:(length(x_data) - 1)
            if j == 1 || j == 4
                % First part and fourth part: dashed line
                plot(x_data(j:j+1), y_data(j:j+1), 'Color', [lighter_color, alpha], 'LineWidth', 2, 'LineStyle', '--');
            else
                % Other parts: solid line
                plot(x_data(j:j+1), y_data(j:j+1), 'Color', [lighter_color, alpha], 'LineWidth', 2);
            end
        end
        
        % Plot markers with lighter color
        plot(x_data, y_data, 'o', 'Color', lighter_color, 'MarkerFaceColor', lighter_color, ...
             'MarkerSize', 3, 'LineWidth', 0.3);
    else
        % Curves 9-10: prominent, bold with dashed pattern
        if i == 9
            curve_color = [0/255, 114/255, 189/255]; % 0x0072BD
        else
            curve_color = [217/255, 83/255, 25/255]; % 0xD95319
        end
        
        % Plot each part of the curve
        for j = 1:(length(x_data) - 1)
            if j == 1 || j == 4
                % First part and fourth part: dashed line
                plot(x_data(j:j+1), y_data(j:j+1), 'Color', curve_color, 'LineWidth', 3, 'LineStyle', '--');
            else
                % Other parts: solid line
                plot(x_data(j:j+1), y_data(j:j+1), 'Color', curve_color, 'LineWidth', 3);
            end
        end
        
        % Plot markers with darker color
        darker_color = curve_color * 0.7; % Make color darker
        plot(x_data, y_data, 'o', 'Color', darker_color, 'MarkerFaceColor', darker_color, ...
             'MarkerSize', 6, 'LineWidth', 1);
    end
end

% Customize the plot
grid on;

% Remove tick labels to show only tick marks
set(gca, 'XTickLabel', []);
set(gca, 'YTickLabel', []);

hold off;

% Calculate bar data
% Set 1: Bar1-1 = max y value of curve 1, Bar1-2 = max y value of curve 2
bar1_1 = max(curve_data{1}(2, :));
bar1_2 = max(curve_data{2}(2, :));

% Set 2: Bar2-1 = max y value of curve 3, Bar2-2 = max y value of curve 4
bar2_1 = max(curve_data{3}(2, :));
bar2_2 = max(curve_data{4}(2, :));

% Set 3: Bar3-1 = max y value of curve 5, Bar3-2 = max y value of curve 6
bar3_1 = max(curve_data{5}(2, :));
bar3_2 = max(curve_data{6}(2, :));

% Set 4: Bar4-1 = max y value of curve 7, Bar4-2 = max y value of curve 8
bar4_1 = max(curve_data{7}(2, :));
bar4_2 = max(curve_data{8}(2, :));

% Create bar chart
figure(2);
bar_data = [bar1_1, bar1_2; bar2_1, bar2_2; bar3_1, bar3_2; bar4_1, bar4_2];
bar(bar_data);

% Customize the bar chart
% legend('Bar-1', 'Bar-2', 'Location', 'best');
grid on;

% Remove tick labels to show only tick marks
set(gca, 'XTickLabel', []);
set(gca, 'YTickLabel', []);

% Display the calculated values
fprintf('Bar values:\n');
fprintf('Set 1: Bar1-1 = %.3f, Bar1-2 = %.3f\n', bar1_1, bar1_2);
fprintf('Set 2: Bar2-1 = %.3f, Bar2-2 = %.3f\n', bar2_1, bar2_2);
fprintf('Set 3: Bar3-1 = %.3f, Bar3-2 = %.3f\n', bar3_1, bar3_2);
fprintf('Set 4: Bar4-1 = %.3f, Bar4-2 = %.3f\n', bar4_1, bar4_2);

% Print RGB values for all curves in hex format
fprintf('\nRGB values for curves (hex format):\n');
for i = 1:10
    if i <= 8
        % Curves 1-8: lighter colors
        lighter_color = colors(i, :) * 0.9 + 0.1;
        r = round(lighter_color(1) * 255);
        g = round(lighter_color(2) * 255);
        b = round(lighter_color(3) * 255);
    else
        % Curves 9-10: special colors
        if i == 9
            curve_color = [0/255, 114/255, 189/255]; % 0x0072BD
        else
            curve_color = [217/255, 83/255, 25/255]; % 0xD95319
        end
        r = round(curve_color(1) * 255);
        g = round(curve_color(2) * 255);
        b = round(curve_color(3) * 255);
    end
    fprintf('Curve %d: #%02X%02X%02X\n', i, r, g, b);
end 