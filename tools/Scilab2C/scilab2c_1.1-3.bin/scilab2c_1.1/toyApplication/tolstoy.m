function [yf, yt, ydiff] = tolstoy(x, h, L)
% compute the convolution between two signals using the time-partitioned
% overlap-save algorithm.
% x -- signal (vector)
% h -- impulse response (vector)
% L -- fft size (IR frames are L/2, signal stepping is L/2 with 50% overlap)
% yf -- result using frequency domain convolution
% yt -- result using time domain convolution
% ydiff -- error between yf and yt
%
% @author gregor heinrich :: arbylon
% @date Nov./Dec. 2006

% TODO: time-varying impulse responses.
% TODO: trade-off calculation for frame sizes.

close all

% standard values
if nargin==0
    L = 2^5;
    x=ones(20,1);
    h=ones(3,1);
    %breaks truncation rule
    L = 2^5;
    x=ones(112,1);
    h=ones(38,1);
end

x = x(:);
h = h(:);

fprintf('signal length: %i, IR length: %i\n', length(x), length(h));
fprintf('1. convolution in time domain.\n');
tic
yt = conv(x,h);
t1 = toc;

%plot(yt);
%title('time-domain convolution y_t');
%figure

K = L / 2;

fprintf('2. convolution in frequency domain.\n');
fprintf('signal frame length: %i / stepping: %i, IR frame length: %i\n', L, K, K);

tic
yf = tols(x, h, L);
t2 = toc;
fprintf('comparison of calculation time: time-domain: %gs, freq-domain: %gs.\n', t1, t2);
plot(yf);
title('frequency-domain convolution y_f');
%figure
len = min([length(yt),length(yf)]);
ydiff = yf(1:len)-yt(1:len);
%plot(ydiff);
%title('error plot y_t - y_f');
error = sum(ydiff.^2);
fprintf('total squared error: %g.\n', error);
fprintf('done.\n');

end % test

