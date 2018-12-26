function [ ang_map ] = fun_map( ang_fft, samp_rate, lower_hz, upper_hz)
% Map ang_fft points to real 0 - sample_rate (Hz) and pick up the frequency
% range we want.
    ang_fft = abs(ang_fft);    
    len = length(ang_fft);
    upper_point = ceil((len-1) / samp_rate * upper_hz + 1);
    lower_point = floor((len-1) / samp_rate * lower_hz + 1);
    ang_map = ang_fft(lower_point:upper_point);
    
end

