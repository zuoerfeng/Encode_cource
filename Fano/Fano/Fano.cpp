//头文件
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

//全局变量
static char p[11][10];
static int flag1=0;
static int ncount1;

//主函数
void main()
{
	void Fano(int m,int n,float y[10]);
	int i,j,flag,ncount;
	float a[11],temp,s=0.0,code_length=0.0;
	flag=0;
	do
	{
		printf("Note: Quit the program when ncount=0 !!!\n");   
		printf("Please input the amount of probabilities,ncount:\n");//请输入量的可能性
		scanf("%d",&ncount);
		if(ncount==0) exit(0);



		while(ncount<0)
		{
			printf("Please input the positive integer! Try again!\n");  //请输入正整数!再试一次!
			printf("Note: Quit the program when ncount=0 !!!\n\n");
			printf("Please input the amount of probabilities,ncount:\n");
			scanf("%d",&ncount);
			if(ncount==0) exit(0);
		}


		while(flag==0)
		{
			flag=1;
			printf("Please all probability!\n");   //请所有的概率
			for(i=1;i<ncount+1;i++)
			{
				scanf("%f",&a[i]);
				if((a[i]<1e-10)||(a[i]-1>1e-10))
				{
					printf("Please input the number which is between 0 and 1\n");//请输入0和1之间的数字
					flag=0;
				}  
				else 
					s=s+a[i];
				if(s-1>1e-10)
				{ 
					printf("Please make sure that the sum of all numbers is 1 !!\n"); //请确保所有数字之和为1
					flag=0;
					s=0;
				}
	
			}
		}
		//冒泡排序
		for(i=1;i<ncount;i++)
		{
			for(j=i+1;j<ncount+1;j++)
			{
				if(a[i]<a[j])
				{
					temp=a[i];
					a[i]=a[j];
					a[j]=temp;
				}
			}
		}
		Fano(1,ncount,a);

		printf("\n\nThe fano code is:\n\n");
		printf("Probability\t\tFano code\t\tcode length\n\n");

		for(i=1;i<ncount+1;i++)
		{
		
			printf("  %.3f\t\t\t",a[i]);
			printf("%s",p[i]);
			printf(" \t\t\t %d\n",strlen(p[i]));
			code_length=code_length+strlen(p[i]);
		}
		printf("\n The average code length is :%.3f \n",code_length/ncount);
		printf("\n\n");
		flag1=0;
		code_length=0.0;

	}while(1);
	exit(0);
}



//费诺编码
void Fano(int m,int n,float y[11])
{  
	int j,k;
	float sum=0.0,s=0.0,s1,z[11];
	flag1++;


	if(flag1==1) 
		ncount1=n;
	if(m==n) 
	{
		return;
	}
	for(j=1;j<ncount1+1;j++)
	{
		z[j]=y[j];
	}
	for(j=m;j<=n;j++)
	{
	   sum=sum+z[j];
	   k=m;
	}
	do
	{ 
		s1=s;
		s=s+y[k++];
	}while(s<=sum-s);

	if((sum-2*s1)<=(2*s-sum))
	{
	   k--;
	}
	for(j=m;j<k;j++)
	{
       strcat(p[j],"0");
	}
	for(j=k;j<=n;j++)
	{
       strcat(p[j],"1");
	}
	 Fano(m,k-1,y);
	 Fano(k,n,y);
	 
}









