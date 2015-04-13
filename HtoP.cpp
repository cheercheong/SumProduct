#include "SumProduct.h"
using namespace  std;

extern unsigned char P_MAT[H_row][H_col-H_row];
extern int rearranged_col[H_row];
extern unsigned char H_check[H_row][H_col];

void arrange_col(int j,int x);
void HtoP();

void arrange_col(int j,int x) // Exchange cols j and x 
{
  char temp;
  for(int k=0;k<H_row;k++)
  {
     temp=H_check[k][j];
     H_check[k][j]=H_check[k][x];
     H_check[k][x]=temp;
  }
}

void HtoP()  // H[n-k][n] = I[n-k][n-k] | P[n-k][k], Gaussian Elimination find P  
{
	    cout<<"\nGaussian Elimination Start\n";
    	int i,j;
		for(i=0;i<H_row;i++)
		{
			cout<<i<<" ";
			int x=i;
			bool flag=false;

			if(i==(H_row-1))
			{
				for(x=i;x<H_col;x++)
				{
                    if(H_check[i][x]==1)
						break;
				}
			}
			else
			{
			  if(H_check[i][i]==0)
			  {
				while((x<H_col))
				{
				
				   for(j=i;j<H_row;j++)
				   { 
					  if(H_check[j][x]==1)
					  {
						  if(x==i)
						  {flag=true;
						  break;}
						  else
						  {
							  if(x>=H_row)
							  {flag=true;
						      break;}
						  }
					  }
				   }
				    if(flag==true)
					break;
                    x++;
				}
			  }
			}
			if(x>=H_col)
			{
				cout<<"Elimination cannot be complished!\n";
			    exit(0);
			}

			if(x!=i)
			{
			  rearranged_col[i]=x;
              arrange_col(i,x);
			}
			else
			{rearranged_col[i]=0;}

		   if(H_check[i][i]==0)
		   {  
			   flag=false;
				   for(j=i+1;j<H_row;j++)
				   {
					  if(H_check[j][i]==1)
					  {
                        flag=true;
						break;
					  }
				   }
               for(int k=0;k<H_col;k++)
			   {
                   H_check[i][k]=(H_check[j][k])^(H_check[i][k]);
			   }
		   }
	   
		   for(j=0;j<H_row;j++)
		   {          
					  if(H_check[j][i]==1)
					  {
						  if(j!=i)
						  {
                             for(int k=0;k<H_col;k++)
							 {
                               H_check[j][k]=(H_check[i][k])^(H_check[j][k]);
							 }
						  }
					  }
		   }          
		}

 		ofstream Pcheck_mat("P_MAT.txt",ios_base::out); 
		for(i=0;i<H_row;i++)
		{
			Pcheck_mat<<(i+1);
            for(j=H_row;j<H_col;j++)
			{
				if(H_check[i][j]==1)
				{
					Pcheck_mat<<','<<(j-H_row+1);
					P_MAT[i][j-H_row]=1;
				}
				else
				{
					P_MAT[i][j-H_row]=0;
				}
			}
            Pcheck_mat<<"\n";
		} 
		Pcheck_mat.close();

		ofstream Change_col("Gaussian_exchange_cols.txt",ios_base::out);
		for(i=0;i<H_row;i++)
		{
			Change_col<<rearranged_col[i]<<'\n';
		}
		Change_col.close();
		cout<<endl<<"Gaussian Elimination Finished"<<endl;
}

 
