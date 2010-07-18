%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [y, fullframes, loops] = tols(x, h, L)
% perform time-partitioned overlap save algorithm
% x -- signal (vector)
% h -- impulse response (vector)
% L -- fft size (frame length for impulse response segmentation is L/2)
% y -- convolution result
% fullframes -- number of full signal frames processed
% loops -- number of total loops
%
% @author gregor heinrich :: arbylon
% @date Nov./Dec. 2006



% fft size for signal and stepping
% (only for notation)
K = L / 2;
% (only for notation; is always = K)
S = L - K;

% segment impulse response
[HH, R] = segmentir(h, K);

% original length of x (without zero-padding the first frame)
lenx = length(x);

% zero-padding of first frame ("saved zeroth frame")
x = [zeros(K,1); x];
fprintf('padding %i zeros in front of x\n', K);

y = [];

% window of input signal frame spectra
% @real-time: can be implemented as circular buffer
XX = zeros(L, R);

% number of full frames from the input
% @real-time: unknown
fullframes = floor((length(x)-L)/S) + 1;
fprintf('expect %i full input frames and %i IR frames.\n', fullframes, R);

% signal frames available (including partial ones)
hassignal = 1;
% more tail frames needed to complete convolution
hastail = 1;
i = 0;

% @real-time: we don't know when the signal ends, thus we don't use the
% fullframes variable in a for loop
while hassignal || hastail
    icirc = mod(i, R) + 1;
    % index into x where xx starts and ends (includes zero-padding)
    xxstart = i * S + 1;
    xxend = i * S + L;
    if xxend <= length(x)
        % complete signal frame
        xx = x(xxstart : xxend);
        %         fprintf(' - signal over full frame\n');
    elseif xxstart <= length(x)
        % incomplete signal frame -> must be zero-padded
        % @real-time: signal ending is started
        xx = x(xxstart : end);
        zpad = xxend - length(x);
        xx = [xx; zeros(zpad, 1)];
        fprintf(' - loop %i: signal incomplete, padding %i zeroes\n', i+1, zpad);
    else
        % @real-time: there are no samples from the input left; signal
        % ending is finished; convolution ending is started
        if hassignal
            hassignal = 0;
            % xframes should be exactly = fullframes
            xframes = i - 1;
            fprintf(' - loop %i: signal finished, processing tail only.\n', i + 1);
        end
    end

    % drop oldest frame and add new one
    % @real-time: can be implemented using a circular buffer
    if (i >= R)
        rend = R;
    else
        % before all ir frames are filled
        rend = i + 1;
    end
    if hassignal
        % more signal samples available
        X = fft(xx, L);
        % for debugging with 1:n: X = round(ifft(X));
        XX(:, icirc) = X;
        rstart = 1;
    else
        % @real-time: during convolution ending
        rstart = i - xframes + 1;
    end

    % total length of y
    ylen = lenx + length(h) - 1;
    % end of output larger than expected result?
    yyend = S*(i+1);
    if yyend > ylen
        hassignal = 0;
        hastail = 0;
        loops = i;
    end

    % add most recent frame to convolution result
    if hastail == 1
        yylen = S;
        y = [y; zeros(S,1)];
    else
        yylen = S - (yyend - ylen);
        y = [y; zeros(yylen,1)];
        loops = i;
    end

    % @real-time: loops over r can be done in parallel, also between
    % subsequent loops over i
    for r=rstart:rend
        rcirc = mod(i - (r - 1), R) + 1;
        % calculate partial convolution
        Y = XX(:,rcirc) .* HH(:,r);
        yy = ifft(Y, L);
        % select last L-K points
        yy = yy(S+1:L);
        % add contribution of signal and ir with r frames offset
        y(end-yylen+1:end) = y(end-yylen+1:end) + yy(1:yylen);
    end
    i = i+1;

end % while
fprintf(' - total loops: %i\n', i);
% TODO: make this independent of xlen
%y = y(1:ylen);
endfunction % convolve

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [HH, R] = segmentir(h, K)
% segment impulse response into columns, the last
% frame/column is zero-padded as needed.
% HH -- matrix of impulse response frames
% R -- number of IR frames (=columns of HH)
    L = 2 * K;
    fullframes = ceil(length(h)/K) - 1;
    for i=1:fullframes
        % column-wise IR frames
        hh(:,i) = h(K*(i-1)+1 : K*i);
    end
    if mod(length(h), K) ~= 0
        % zero-pad last ir frame
        hlast = h((fullframes*K+1):length(h));
        hlast = [hlast ; zeros(K - length(hlast),1)];
        hh(:,fullframes+1) = hlast;
    end
    % column ffts
    HH = fft(hh, L);
    % for debugging 1:n: HH = round(ifft(HH));
    R = size(HH,2);
endfunction % segmentir

