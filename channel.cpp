#include "SumProduct.h"
using namespace  std;

  void mgrns(double u,double g,double *r,int n,double *a);

  void mgrns(double u,double g,double *r,int n,double *a)
  { 
	int i,k,m;
    double s,w,v,t;
    s=65536.0; w=2053.0; v=13849.0;
    for (k=0; k<=n-1; k++)
      { t=0.0;
        for (i=1; i<=12; i++)
          { *r=(*r)*w+v; m=(int)(*r/s);
            *r=*r-m*s; t=t+(*r)/s;
          }
        a[k]=u+g*(t-6.0);
      }
    return;
  }

  void add_noise(int * in_adderss,double N0,double * out_adderss)
  {
	  	double h=rand();
		double *AWGN=new double[H_col];
		mgrns(0.0,sqrt(N0/2.0),&h,H_col,AWGN);
			for(int i=0;i<H_col;i++)
			{
				if(*(in_adderss+i)==0)
				{
				 *(out_adderss+i)=(1+(*(AWGN+i)));}
				if(*(in_adderss+i)==1)
				{
				 *(out_adderss+i)=(-1+(*(AWGN+i)));}
			}
		delete AWGN;
  }
