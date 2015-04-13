#include "SumProduct.h"
using namespace  std;

void initi();
extern struct Fnode_inf // Function node structure
{
	bool flag;
	int col_num;
	double q_dmeg;
	double r_meg0; 
	double r_meg1;

};
extern struct Vnode_inf // Variable node structure 
{
	bool flag;
	int Fcol_num;
    int Frow_num;
};
extern Fnode_inf Fnode_inf_mat[H_row][Hmax_rowdeg]; // Fnode_inf_mat[i][j]---Function node i's jth edge connected to the variable node col_num 
extern Vnode_inf Vnode_inf_mat[H_col][Hmax_coldeg]; // Vnode_inf_mat[i][j]---Variable node i's jth edge connected to the function node col_num
unsigned char H_check[H_row][H_col]; // Full H matrix
void initi()
{
	int temp_int,i,j;
	char temp_char;
	for(i=0;i<H_col;i++)
	{
		for(int j=0;j<Hmax_coldeg;j++)
		{
			Vnode_inf_mat[i][j].flag=false;
		}
    }
	for(i=0;i<H_row;i++)
	{
		for(j=0;j<H_col;j++)
		{
		  H_check[i][j]=0;
		}
	}
	ifstream check_mat("mat.txt",ios_base::in);  // Read H matrix in sparse form from file
    if(check_mat.is_open()==0)
	{
		cout<<endl<<"Cannot open the H matrix file!"<<endl;
	    exit(0);
	}
	for(i=0;i<H_row;i++) // The ith row of the H matrix
	{
		check_mat>>temp_int;
		j=0;
		while(check_mat.peek()==',') // Read the 1s' column location in row i 
		{
			check_mat>>temp_char;
			check_mat>>temp_int;
			if(temp_int!=0)
			{
				H_check[i][temp_int-1]=1; 
				Fnode_inf_mat[i][j].flag=true; 
				Fnode_inf_mat[i][j].col_num=temp_int-1; // The function node i is linked to the variable node col_num  
				for(int k=0;k<Hmax_coldeg;k++) 
				{
					if(Vnode_inf_mat[temp_int-1][k].flag==false)
					{Vnode_inf_mat[temp_int-1][k].Frow_num=i;
					Vnode_inf_mat[temp_int-1][k].Fcol_num=j;
					Vnode_inf_mat[temp_int-1][k].flag=true;
					break;}
				}
			}
			else
			{
				Fnode_inf_mat[i][j].flag=false;
			}
			//cout<<temp_char<<" "<<temp_int;
			j++;
		}
		//cout<<endl;
	}
	check_mat.close();
}
