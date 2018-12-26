function [ freq1, freq2 ] = fun_getBreathFreq( ang_index_minute )
% Find the possible breath frequency in minutes
% According to experience, we assume the breath frequency is between 12.5 
% and 40 per minute, giving the measured subjects stay still.

% The outputs are the frequencies correspond to the first and the second 
% peaks of the eulerian angle after fft.
    odd_lower = 12.5;
    odd_upper = 40;
    i = 1;
    while (ang_index_minute(i) < odd_lower) || (ang_index_minute(i) > odd_upper)
        i = i + 1;
    end
    freq1 = ang_index_minute(i);
    
    i = i + 1;
    while (ang_index_minute(i) < odd_lower) || (ang_index_minute(i) > odd_upper)
        i = i + 1;
    end
    freq2 = ang_index_minute(i);

end

