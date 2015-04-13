clc 
clear all 

[a,b]=textread('ber_BP.txt','%f%f');
[c,d]=textread('ber_MinSum.txt','%f%f');

semilogy(a,b,'b*-',c,d,'ro-');
%axis([-2.0 2.5 10e-5 10e-2]);
legend('BP','Min-Sum');
xlabel('Eb/No (dB)'); ylabel('BER');
grid on; hold on;
