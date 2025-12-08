% device_num = [50, 75, 150, 225, 450];
% solution_num = [2460, 3130, 9430, 16505, 46782];
% scatter(device_num, solution_num);

rng(13);
device_num = (128:512:8192);
p1 = 0.1314;
p2 = 46;
p3 = -549;
y = p1 * device_num.*device_num + p2 * device_num - p3;
err =  2e5 * randn(size(device_num));
err(1:3) = 0;
err(4) = err(4)/10;
err(5) = err(5)/10;
y = y + err;
scatter(device_num,y, 'filled');