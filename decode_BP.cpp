#include "SumProduct.h"
using namespace  std;

extern struct Fnode_inf 
{
	bool flag;
	int col_num;
	double q_dmeg;  // V->F
	double r_meg0;  // F->V
	double r_meg1;
};
extern struct Vnode_inf 
{
	bool flag;
	int Fcol_num;
    int Frow_num;
};
extern Fnode_inf Fnode_inf_mat[H_row][Hmax_rowdeg];
extern Vnode_inf Vnode_inf_mat[H_col][Hmax_coldeg];

void decode(double *in_address,double N0,int *out_address)
{

	double p0[H_col],p1[H_col];
    int result[H_col]; // Codeword after decoding
	int i,j,k,v;
	for(i=0;i<H_col;i++)
	{
		p0[i]=1/(1+exp(-4.0*(*(in_address+i))/N0));  // Likelihood ratio (LR) for the variable node i
		p1[i]=1-p0[i];
	}
	for(i=0;i<H_row;i++)
	{
		for (j=0;j<Hmax_rowdeg;j++)
		{
			if(Fnode_inf_mat[i][j].flag==true) 
			{
				Fnode_inf_mat[i][j].q_dmeg=p0[Fnode_inf_mat[i][j].col_num]-p1[Fnode_inf_mat[i][j].col_num];
			}
			else
				break;
		}	
	}
	//-----------------------------Iteration Starts---------------------------------//
	for(i=0;i<MAX_cycle;i++)
	{
		//-----------------------Horizontal Iteration------------------------//
		for(j=0;j<H_row;j++) // For every function node j
		{	
			for(k=0;k<Hmax_rowdeg;k++) // For every variable node k 
			{
				double drmn=1.00000; 
				if(Fnode_inf_mat[j][k].flag==false) // Variable node k is not linked to the function node j
					break;
				else
				{
					for(v=0;v<Hmax_rowdeg;v++) // For every variable node v linked to function node j
					{
						if(v!=k) // Multiply all the info received at function node j except that from variable node k
						{
							if(Fnode_inf_mat[j][v].flag==true)
								drmn=drmn*Fnode_inf_mat[j][v].q_dmeg; // Multiply all V'->F = F->V
							else
								break;
						}
					}
					Fnode_inf_mat[j][k].r_meg0=(1+drmn)/2; // Info F->V about V=0
					Fnode_inf_mat[j][k].r_meg1=(1-drmn)/2;
				}
			}
		}
		//-----------------------Vertical Iteration------------------------//
		for(j=0;j<H_col;j++) // For every variable node j
		{
			double store_last0;
            double prod_rmn0,prod_rmn1,const1,const2;
			for(k=0;k<Hmax_coldeg;k++) // For every function node k
			{
				prod_rmn0=1.00000;
				prod_rmn1=1.00000;
				if(Vnode_inf_mat[j][k].flag==false) // Function node k is not linked to the variable node j
					break;
				else
				{
					for(v=0;v<Hmax_coldeg;v++) // For every function node v linked to variable node j
					{
						if(v!=k)
						{
							if(Vnode_inf_mat[j][v].flag==true) // Multiply all F'->V = V->F
							{prod_rmn0=prod_rmn0*Fnode_inf_mat[Vnode_inf_mat[j][v].Frow_num][Vnode_inf_mat[j][v].Fcol_num].r_meg0;
							 prod_rmn1=prod_rmn1*Fnode_inf_mat[Vnode_inf_mat[j][v].Frow_num][Vnode_inf_mat[j][v].Fcol_num].r_meg1;}
							else
								break;
						}
					}
					const1=p0[j]*prod_rmn0; // Multiply LR for V
					const2=p1[j]*prod_rmn1;
					double temp_double=1.0/(const1+const2); // Normalization, so that qmn0+qmn1=1
					Fnode_inf_mat[Vnode_inf_mat[j][k].Frow_num][Vnode_inf_mat[j][k].Fcol_num].q_dmeg=temp_double*(const1-const2); // Update V->F
					store_last0=Fnode_inf_mat[Vnode_inf_mat[j][k].Frow_num][Vnode_inf_mat[j][k].Fcol_num].r_meg0; // F->V
				}
			}
		    if((const1*store_last0)>=(const2*(1-store_last0))) // Summary message for V
				result[j]=0;
			else
				result[j]=1;
		}
		//-----------------------Check if the decoding is successful------------------------//
		int not_fit_fuction=0;
		for(j=0;j<H_row;j++)
		{
			int num_1=0;
			for(k=0;k<Hmax_rowdeg;k++)
			{
				if(Fnode_inf_mat[j][k].flag==true)
				{
					if(result[Fnode_inf_mat[j][k].col_num]==1)
						num_1++;
				}
				else
					break;
			}
			if((num_1%2)!=0)
			{not_fit_fuction++;break;}
		}

		if(not_fit_fuction==0)
		{
			for(j=0;j<H_col;j++)
			{
				*(out_address+j)=result[j];
			}
			break;
		}
    }//-----------------------------Iteration Ends---------------------------------//
	for(j=0;j<H_col;j++)
	{
		*(out_address+j)=result[j];
	}
}
