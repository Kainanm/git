function y = fun_lpf(x)
%FUN_LPF Filters input x and returns output y.

% MATLAB Code
% Generated by MATLAB(R) 8.6 and the DSP System Toolbox 9.1.
% Generated on: 29-Jun-2018 16:23:37

%#codegen

% To generate C/C++ code from this function use the codegen command. Type
% 'help codegen' for more information.

persistent Hd;

if isempty(Hd)
    
    % The following code was used to design the filter coefficients:
    % % Equiripple Lowpass filter designed using the FIRPM function.
    %
    % % All frequency values are in Hz.
    % Fs = 17;  % Sampling Frequency
    %
    % Fpass = 1;               % Passband Frequency
    % Fstop = 2;               % Stopband Frequency
    % Dpass = 0.057501127785;  % Passband Ripple
    % Dstop = 0.001;           % Stopband Attenuation
    % dens  = 20;              % Density Factor
    %
    % % Calculate the order from the parameters using FIRPMORD.
    % [N, Fo, Ao, W] = firpmord([Fpass, Fstop]/(Fs/2), [1 0], [Dpass, Dstop]);
    %
    % % Calculate the coefficients using the FIRPM function.
    % b  = firpm(N, Fo, Ao, W, {dens});
    
    Hd = dsp.FIRFilter( ...
        'Numerator', [0.00131311722728825 0.000540270250441159 ...
        -0.000889133827284966 -0.00409091278428106 -0.00921175653705548 ...
        -0.015719500399212 -0.0222359415830897 -0.0266157100364206 ...
        -0.0263035291044647 -0.0189340372370289 -0.0030528458862749 ...
        0.0212695973820235 0.0521056160696496 0.0857903866996005 ...
        0.117499993240713 0.14220054107663 0.155727644706228 0.155727644706228 ...
        0.14220054107663 0.117499993240713 0.0857903866996005 0.0521056160696496 ...
        0.0212695973820235 -0.0030528458862749 -0.0189340372370289 ...
        -0.0263035291044647 -0.0266157100364206 -0.0222359415830897 ...
        -0.015719500399212 -0.00921175653705548 -0.00409091278428106 ...
        -0.000889133827284966 0.000540270250441159 0.00131311722728825]);
end

y = step(Hd,x);


% [EOF]
