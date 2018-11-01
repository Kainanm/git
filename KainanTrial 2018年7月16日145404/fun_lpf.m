function y = fun_lpf(x)
%FUN_LPF Filters input x and returns output y.

% MATLAB Code
% Generated by MATLAB(R) 8.6 and the DSP System Toolbox 9.1.
% Generated on: 12-Jul-2018 18:49:08

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
    % Fpass = 1.5;             % Passband Frequency
    % Fstop = 2.5;             % Stopband Frequency
    % Dpass = 0.057501127785;  % Passband Ripple
    % Dstop = 0.0001;          % Stopband Attenuation
    % dens  = 20;              % Density Factor
    %
    % % Calculate the order from the parameters using FIRPMORD.
    % [N, Fo, Ao, W] = firpmord([Fpass, Fstop]/(Fs/2), [1 0], [Dpass, Dstop]);
    %
    % % Calculate the coefficients using the FIRPM function.
    % b  = firpm(N, Fo, Ao, W, {dens});
    
    Hd = dsp.FIRFilter( ...
        'Numerator', [-0.000432968108717059 -0.00140765600882177 ...
        -0.00307208470538723 -0.00507583399824286 -0.00651386126630115 ...
        -0.00608279210663509 -0.00262294123528309 0.0040996502053513 ...
        0.0127339186161362 0.0202636565151811 0.0228171801858489 ...
        0.0172944759319146 0.00326102151472989 -0.0157704037158572 ...
        -0.0324992203964246 -0.0377776039614552 -0.0238906006086383 ...
        0.011993001087039 0.0656674635287373 0.126161646152355 0.178653612860713 ...
        0.20913670626946 0.20913670626946 0.178653612860713 0.126161646152355 ...
        0.0656674635287373 0.011993001087039 -0.0238906006086383 ...
        -0.0377776039614552 -0.0324992203964246 -0.0157704037158572 ...
        0.00326102151472989 0.0172944759319146 0.0228171801858489 ...
        0.0202636565151811 0.0127339186161362 0.0040996502053513 ...
        -0.00262294123528309 -0.00608279210663509 -0.00651386126630115 ...
        -0.00507583399824286 -0.00307208470538723 -0.00140765600882177 ...
        -0.000432968108717059]);
end

y = step(Hd,x);


% [EOF]
