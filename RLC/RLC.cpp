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

void Youchen::shuru_xulie()//��������
{
	int i;m=0;
    cout<<"����������(��.����!):"<<endl;
    for(i=0;;i++)
	{cin>>array[i];
     m+=1;
     if(array[i]=='.')
     break;
	}
}

void Youchen::bianma_xulie()//���γ̱��뷽�����б���
{
	int i,j,sum=0;
	cout<<"��������У�";
    for(i=0;i<=m-1;i++)
	  {
		if(array[i]==array[i+1])//����ͬ�ַ����м���
		    sum++;
		else//���ǰ�����ַ��������������
		{
			if(sum<3)
			{
			  for(j=0;j<=sum;j++)
			  cout<<array[i];
			}
			else
			{cout<<"("<<array[i]<<sum+1<<")";}
			sum=0;//����һ����ͬ�ַ����¿�ʼ����
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