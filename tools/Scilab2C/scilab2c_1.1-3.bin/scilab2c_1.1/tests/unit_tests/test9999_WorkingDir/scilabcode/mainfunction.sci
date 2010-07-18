//SCI2C: DEFAULT_PRECISION= DOUBLE
function mainfunction()
//---> MOREA: commentare meglio lo scopo di questa funzione
//---> NUTRICATO: annotare anche le funzioni di plot facendole puntare a funzioni dummy che non fanno nulla.
//---> NUTRICATO: la convoluzione e' stata stostituita con una custom in quanto non ancora implementata da INRIA.


// sampling parameters
fs   = 1000; //Hz
Tmax = .01; //----> NUTRICATO: VALORE VERO 1 
t    = 0 : 1 ./ fs : Tmax;

// chirp parameters
fo = 10
k  = 150

tau=0.7

c=exp(2*%i*%pi*(fo*t+k./2*t.^2));//chirp signal in complex form
// figure
// subplot(2,2,1)
// plot(t(1:250),real(c(1:250)));
//title('chirp(t)')

cr=exp(2*%i*%pi*(fo*(t-tau)+k./2*(t-tau).^2));//chirp signal delayed in complex form
// subplot(2,2,2)

// plot(t(1:250),real(c(1:250)),'r',t(1:250),real(cr(1:250)),'g');
// title('chirp(t) e chirp(t-tau)')

// filter response 
// subplot(2,2,3)
h=conj(exp(2*%i*%pi*(fo*(-t)+k./2*(-t).^2)));


// plot(t(1:250),real(h(1:250)));
// title('conj(chirp(-t))')	
//---> NUTRICATO: la convoluzione e' stata commentata in quanto non ancora implementata.
y=myconvol(h,cr);


tch=0:1 ./ fs:2*Tmax;
// subplot(2,2,4)
// plot(tch,abs(y))
// title(' y(t)')

disp('t')
disp(t)
disp('c')
disp(c)
disp('cr')
disp(cr)
disp('h')
disp(h)
disp('y')
disp(y)
endfunction


