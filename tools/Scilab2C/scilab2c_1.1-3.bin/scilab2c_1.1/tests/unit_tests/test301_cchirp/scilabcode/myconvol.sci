//SCI2C: NIN=          2
//SCI2C: NOUT=         1
//SCI2C: OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)
//SCI2C: OUT(1).SZ(1)= '1'
//SCI2C: OUT(1).SZ(2)= FA_SUB(FA_ADD(IN(1).SZ(2),IN(2).SZ(2)),'1')
//SCI2C: DEFAULT_PRECISION= DOUBLE

function y = myconvol(h,x)
hlen = length(h);
xlen = length(x);
//y = zeros(1,hlen+xlen-1);

for cnt1 = 1:xlen
   y(cnt1) = 0+%i*0;
   for cnt2 = 1:cnt1
      if ((cnt1-cnt2+1) <= hlen)
         y(cnt1) = y(cnt1) + h(cnt1-cnt2+1)*x(cnt2);
      else
         y(cnt1) = y(cnt1) + 0;
      end
   end
end

for cnt1 = xlen+1:xlen+hlen-1
   y(cnt1) = 0+%i*0;
   for cnt2 = cnt1-hlen+1:xlen
      y(cnt1) = y(cnt1) + h(cnt1-cnt2+1)*x(cnt2);
   end
end

endfunction
