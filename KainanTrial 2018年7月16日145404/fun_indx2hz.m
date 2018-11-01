function [ ang_max_index_hz ] = fun_indx2hz( ang_max_index, range_lower, range_upper)
% % map index to Hz based on given freuquency range
    m = (range_upper - range_lower)/(length(ang_max_index)-1);
    ang_max_index = ((ang_max_index-1) .* m + range_lower); % map to Hz (for MATLAB)% Ƶ��Ϊ15.304 
    % ang_max_index = ((ang_max_index) .* m + range_lower); % map to Hz (for C language)% Ƶ��Ϊ16.072
    ang_max_index_hz = ang_max_index .* 60;
end

