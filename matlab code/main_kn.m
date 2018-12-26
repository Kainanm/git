load('Pitch.mat');
load('Roll.mat');
load('Yaw.mat');
% N = length(Pitch);
N = 256;
M = 128;
samp_rate = 4;
offset = 0;
rag = 1+offset*M : N+offset*M;

Pitch = Pitch(rag);
Yaw = Yaw(rag);
Roll = Roll(rag);

% Calculate variance
pitch_var = var(Pitch);
yaw_var = var(Yaw);
roll_var = var(Roll);

% Order angles in descending
ang_mat = fun_angdes(Pitch, Yaw, Roll, pitch_var, yaw_var, roll_var); % 测试数据最大为Roll>Yaw>Pitch

% Test of filter
ang_max_filt = fun_lpf(ang_mat(:,1));
ang_mid_filt = fun_lpf(ang_mat(:,2));
ang_min_filt = fun_lpf(ang_mat(:,3));

% Test of detrend

% ang_max_detrd = detrend(ang_max_filt);
% ang_mid_detrd = detrend(ang_mid_filt);
% ang_min_detrd = detrend(ang_min_filt);

ang_max_detrd = fun_detrend(ang_max_filt);
ang_mid_detrd = fun_detrend(ang_mid_filt);
ang_min_detrd = fun_detrend(ang_min_filt);

ang_max_fft = abs(fft(ang_max_detrd));
ang_mid_fft = abs(fft(ang_mid_detrd));
ang_min_fft = abs(fft(ang_min_detrd));

lower_hz = 0.2;
upper_hz = 1.25;
ang_max_map = fun_map(ang_max_fft, samp_rate, lower_hz, upper_hz);
ang_mid_map = fun_map(ang_mid_fft, samp_rate, lower_hz, upper_hz);
ang_min_map = fun_map(ang_min_fft, samp_rate, lower_hz, upper_hz);

[ang_max_sort, ang_max_index] = sort(ang_max_map, 'descend');
ang_max_index_minute = fun_indx2hz(ang_max_index, lower_hz, upper_hz);
[ang_mid_sort, ang_mid_index] = sort(ang_mid_map, 'descend');
ang_mid_index_minute = fun_indx2hz(ang_mid_index, lower_hz, upper_hz);

[freq1, freq2] = fun_getBreathFreq(ang_max_index_minute);
breath_freq = (0.7 * freq1 + 0.3 * freq2);
disp('Fourier变换求出的可能的呼吸频率（次/分钟）：'),disp(breath_freq), ...
    disp('当输入长度为：'),disp(N);



