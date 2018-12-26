load('Pitch.mat');
load('Roll.mat');
load('Yaw.mat');
N = 1024;
M = 128;

data = [Pitch(N+1:N+M) Yaw(N+1:N+M) Roll(N+1:N+M)]';