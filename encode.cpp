#include "SumProduct.h"
using namespace  std;

void arrange_col(int j,int x);

extern unsigned char P_MAT[H_row][H_col-H_row];
extern int rearranged_col[H_row];

void encode(int *c_msg)
{
	    unsigned char signal_bit[H_col-H_row],check_bit[H_row],total_bit[H_col];
		int temp_int,i,j;
		unsigned char temp_char;
		    
			for(i=0;i<H_col-H_row;i++)  // Randomly generate k (=H_col-H_row) signal bits
			{
				if((((int)(rand()))%2)==0)
					signal_bit[i]=0;
				else
					signal_bit[i]=1;
			}				
			for(i=0;i<H_row;i++) // Calculate check bits according to parity check matrix
			{
				for(j=0;j<H_col-H_row;j++) // For every k-bit message, calculate the (n-k)-bit check bits
				{
					if(j==0)
						temp_char=signal_bit[j]&P_MAT[i][j];
					else
						temp_char=temp_char^(signal_bit[j]&P_MAT[i][j]);
				}
				check_bit[i]=temp_char;
			}	

			for(i=0;i<H_col;i++) // n_total_bit = (n-k)_check_bit + k_signal_bit
			{
				if(i<H_row)
					total_bit[i]=check_bit[i];
				else
					total_bit[i]=signal_bit[i-H_row];
			}

			for(i=H_row-1;i>=0;i--) // Rearrange the signal bits to the original order
			{
				if(rearranged_col[i]!=0)
				{   
					temp_int=rearranged_col[i];
					temp_char=total_bit[i];
					total_bit[i]=total_bit[temp_int];
					total_bit[temp_int]=temp_char;
				}
			}
		
			for(i=0;i<H_col;i++) // Record the output codeword
			{
				*(c_msg+i)=(int)total_bit[i];				
			}
}
