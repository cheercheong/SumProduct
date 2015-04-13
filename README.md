# SumProduct
Implement of Sum-Product algorithm and its variation Min-Sum algorithm in C++
There are two projects "BP" and "Min_Sum" for the corresponding two algorithms.
The given H matrix is written in the "mat.txt" in sparse form.
/**********************************************************/
The projects each contains :

SumProduct.h 
Main_SumProduct_BP.cpp / Main_SumProduct_MinSum.cpp
initi_BP.cpp / initi_MinSum.cpp
HtoP.cpp
encode.cpp
channel.cpp
decode_BP.cpp / decode_MinSum.cpp
/**********************************************************/
The output files are:

Gaussian_exchange_cols.txt
P_MAT.txt
ber_BP.txt / ber_MinSum.txt
/**********************************************************/
"SumProduct.m" is the executable matlab file that reads "ber_BP.txt" and "ber_MinSum.txt" and plot the SNR-BER curves.

Simulation parameters can be changed in SumProduct.h, however, the "mat.txt" should be changed correspondingly.




