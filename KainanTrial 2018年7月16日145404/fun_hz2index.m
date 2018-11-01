function [ index ] = fun_hz2index(hz, len, fs)
% To convert the frequency argument to index of the cell, given sample
% rate and length of the cell.
% The function is for C language, considering index of cells start from 0.

m = len / (fs/2);
index = round(m * hz);

end