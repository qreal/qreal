//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//the program construct moebius 
// and klein bottle surface from
//http://www.metlin.org/mathematics/
// topological-constructs/non-orientable-surfaces/


// ---------------------
// --- Moebius strip ---
// ---------------------
num_points = 20;
R          = 20;
a          = -0.4;
b          = 0.4;

step=(b-a)./(num_points-1);
step1=(2*%pi)./num_points;
s=a:step:b;
t=0:step1:2*%pi;

[S, T] =  XYgrid(s, t);
x = [R + (S.*cos(T./2))].*cos(T);
y = [R + (S.*cos(T./2))].*sin(T);
z = S.*sin(T./2);

disp('x(1:10)')
disp(x(1:10))
disp('y(1:10)')
disp(y(1:10))
disp('z(1:10)')
disp(z(1:10))

// ENABLE/DISABLE PLOT
// scf(0); surf(x, y, z);

// -----------------------
// --- Klein Bottle II ---
// -----------------------
num_points = 50;

//u = linspace (0, 2*(%pi), num_points);
//v = linspace (0, 2*(%pi), num_points);
step_u = (2*%pi)./num_points 
u = 0:step_u:2*%pi;
v = u;

[U,V] = XYgrid(u,v);

x1 = cos(U).*(cos(U./2).*(sqrt(2)+cos(V))+(sin(U./2).*sin(V).*cos(V)));
y1 = sin(U).*(cos(U./2).*(sqrt(2)+cos(V))+(sin(U./2).*sin(V).*cos(V)));
z1 = -1.*sin(U./2).*(sqrt(2)+cos(V))+cos(U./2).*sin(V).*cos(V);

disp('x1(1:10)');
disp(x1(1:10));
disp('y1(1:10)')
disp(y1(1:10))
disp('z1(1:10)')
disp(z1(1:10))
// ENABLE/DISABLE PLOT
// scf(1) ;surf(x1, y1, z1)
    
endfunction

