clear all 
close all
N = 256;
SAMP_RATE = 17;
PI2 = 6.28318530717959;
indata = zeros(1,N);
for i = 0:N-1
     indata(i+1) = 5*sin(PI2 * i * 1.0 / SAMP_RATE) + ...
               sin(PI2 * i * 2.0 / SAMP_RATE) + ...
               sin(PI2 * i * 7.0 / SAMP_RATE);
end
figure()
t = 0:N/SAMP_RATE/(N-1):N/SAMP_RATE;
plot(t,indata)

y = abs(fft(indata));
y_fil = abs(fft(fun_lpf(indata)));
x = -SAMP_RATE/2:SAMP_RATE/(length(indata)-1):SAMP_RATE/2;

figure()
plot(x,fftshift(y));

figure()
plot(t, fun_lpf(indata));

figure()
plot(x,fftshift(y_fil));
