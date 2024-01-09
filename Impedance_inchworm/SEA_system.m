%Series Elastic Actuator  


   %                                       c
   %          |=====|    k_sp     |=====|--]--
   %       C  |  J  |--/-/-/-/-/--|  j  |
   %     --]--|=====|             |=====|
   % ______________________________________

% link value
J_link = 10;
C_link = 10;

% laod value
J_load = J_link;
C_load = C_link;

% spring value
k_sp = 1;

J = J_link;
j = J_load;
C = C_link;
c = C_load;


% PID gain
k_p = 1;
k_d = 0.1;

%Transfer function
num = [0 k_d*j k_d*c+j*k_p c*k_p-k_d*k_sp -k_sp*k_p];
den = [J*j J*c+j*C C*c-J*k_sp-j*k_sp -c*k_sp-C*k_sp 0]+[0 k_d*j k_d*c+j*k_p c*k_p-k_d*k_sp -k_sp*k_p]

tf(num,den)

%stabillity
rlocus(num, den)

