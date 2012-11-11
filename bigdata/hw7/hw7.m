function [r2m r2s thetam thetas] = hw7( data )
%HW7 Summary of this function goes here
%   Detailed explanation goes here

% parse input
M = data(:, 1);
S = data(:, 2);
feat = data(:, 3:end);

% feature normalization
[n d] = size(feat);
% feat = feat - repmat(mean(feat), [n 1]);
% feat = feat ./ repmat(std(feat), [n 1]);

% compute r2
for i = 1 : d
    r2m(i) = r2_compute(feat(:, i), M);
    r2s(i) = r2_compute(feat(:, i), S);
end

[r2m(d+1) thetam] = r2_compute(feat, M);
[r2s(d+1) thetas] = r2_compute(feat, S);

end

function [r2 theta] = r2_compute(X, Y)
[n d] = size(X);
X = [ones(n, 1) X];
theta =  pinv(X' * X) * X' * Y;
P = X * theta;
r2 = 1 - (sum((P - Y).^2) / sum((Y - mean(Y)).^2));
end

