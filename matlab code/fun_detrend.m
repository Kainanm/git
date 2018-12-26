function [ y_detrd ] = fun_detrend(y)
% To replace the detrend() in matlab
% by using linear regression for polyfit
    n = length(y);
    x = 0 : n-1;
    sum_x = sum(x);
    sum_y = sum(y);
    sum_xy = x * y; % x is row vector, y is column vector
    sum_xx = sum(x.^2);
    a = (n * sum_xy - sum_x * sum_y)/(n * sum_xx - sum_x * sum_x);
    
    avr_x = sum_x / n;
    avr_y = sum_y / n;
    b = avr_y - a * avr_x;
    
    y_detrd = y - (x' * a + b);
end

