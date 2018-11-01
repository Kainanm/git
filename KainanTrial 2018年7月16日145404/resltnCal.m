function [] = resltnCal(samp_rate, buff_len, fft_len)
% input is sample rate, buff length and fft length
    point_period = 1/samp_rate; % sec/point
    cal_period = buff_len/samp_rate; % sec
    resolution = samp_rate/fft_len * 60; % times/min
    disp(['Sensor data is sampled every ',num2str(point_period),' second per point']);
    disp(['Breath rate result is updated every ', num2str(cal_period), ' second']);
    disp(['Resolution of the program can be ', num2str(resolution), ' times per miniute']);
end