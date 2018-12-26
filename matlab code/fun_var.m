function [variance] = fun_var(X)
% find the variance of a cell
    variance = sum((X(1,:)-mean(X).^2)/length(X));
end

