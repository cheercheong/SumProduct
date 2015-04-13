#include "SumProduct.h"
using namespace  std;

#define  sgn(x)     ( (x)==0.0 ? 0.0 : ((x)<0.0 ? -1.0 : 1.0))
#define  abs(x)     ( fabs(x))
#define  min(x,y)   ((x)<(y) ?  x:y)

extern struct Fnode_inf 
{
	bool flag;
	int col_num;
	double q_dmeg;  // V->F
	double r_meg;  // F->V
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

	double pl[H_col];

    int result[H_col]; // Codeword after decoding
	int i,j,k,v;
	for(i=0;i<H_col;i++)
	{
		pl[i]=4 * (*(in_address+i)) / N0; //Log Likelihood ratio (LLR) for the variable node i
	}
	for(i=0;i<H_row;i++)
	{
		for (j=0;j<Hmax_rowdeg;j++)
		{
			if(Fnode_inf_mat[i][j].flag==true) 
			{
				Fnode_inf_mat[i][j].q_dmeg=pl[Fnode_inf_mat[i][j].col_num];
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
				double minbeta = 10000.0;
				double sgnbeta = 1.0;
				if(Fnode_inf_mat[j][k].flag==false) // Variable node k is not linked to the function node j
					break;
				else
				{
					for(v=0;v<Hmax_rowdeg;v++) // For every variable node v linked to function node j
					{
						if(v!=k) // Multiply all the info received at function node j except that from variable node k
						{
							if(Fnode_inf_mat[j][v].flag==true)
							{
								minbeta = min(abs(Fnode_inf_mat[j][v].q_dmeg),abs(minbeta));
								sgnbeta*=sgn(Fnode_inf_mat[j][v].q_dmeg);
							}
							else
								break;
						}
					}
					Fnode_inf_mat[j][k].r_meg=sgnbeta*minbeta; // Info F->V about V=0
				}
			}
		}
		//-----------------------Vertical Iteration------------------------//
		for(j=0;j<H_col;j++) // For every variable node j
		{
			double store_last0;
            double prod_rmn,const1;
			for(k=0;k<Hmax_coldeg;k++) // For every function node k
			{
				prod_rmn=0.00000;
				if(Vnode_inf_mat[j][k].flag==false) // Function node k is not linked to the variable node j
					break;
				else
				{
					for(v=0;v<Hmax_coldeg;v++) // For every function node v linked to variable node j
					{
						if(v!=k)
						{
							if(Vnode_inf_mat[j][v].flag==true) // Multiply all F'->V = V->F
							{prod_rmn=prod_rmn+Fnode_inf_mat[Vnode_inf_mat[j][v].Frow_num][Vnode_inf_mat[j][v].Fcol_num].r_meg;}
							else
								break;
						}
					}
					const1=pl[j]+prod_rmn; // Multiply LR for V
					Fnode_inf_mat[Vnode_inf_mat[j][k].Frow_num][Vnode_inf_mat[j][k].Fcol_num].q_dmeg=const1; // Update V->F
					store_last0=Fnode_inf_mat[Vnode_inf_mat[j][k].Frow_num][Vnode_inf_mat[j][k].Fcol_num].r_meg; // F->V
				}
			}
		    if((const1+store_last0)>=0) // Summary message for V
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
