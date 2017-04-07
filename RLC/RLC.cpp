#include<iostream>
using namespace std;


class Youchen
{
public:
   void shuru_xulie();
   void bianma_xulie();
private:
	char array[1000];
	int m;
};

void Youchen::shuru_xulie()//输入序列
{
	int i;m=0;
    cout<<"请输入序列(以.结束!):"<<endl;
    for(i=0;;i++)
	{cin>>array[i];
     m+=1;
     if(array[i]=='.')
     break;
	}
}

void Youchen::bianma_xulie()//用游程编码方法进行编码
{
	int i,j,sum=0;
	cout<<"编码后序列：";
    for(i=0;i<=m-1;i++)
	  {
		if(array[i]==array[i+1])//对相同字符进行记数
		    sum++;
		else//如果前后两字符不相等则进行输出
		{
			if(sum<3)
			{
			  for(j=0;j<=sum;j++)
			  cout<<array[i];
			}
			else
			{cout<<"("<<array[i]<<sum+1<<")";}
			sum=0;//从另一个不同字符重新开始记数
		}
	  }
	cout<<endl;	  
}
int main()
{
	Youchen xulie;
	xulie.shuru_xulie();
	xulie.bianma_xulie();
	return 0;
}