#include<stdio.h>
#include<string.h>

char dic[30][50];
int n;

void init()							//�ֵ��ʼ��
{
	int i;
	strcpy(dic[0],"a");				//��ʼʱ�ʵ�������п��ܵĸ�
	strcpy(dic[1],"b");
	strcpy(dic[2],"c");
	for(i = 3; i < 30; i++)			//����Ϊ��
		strcpy(dic[i],"");
}

int find(char s[])					//�ֵ���Ѱ��,�������
{
	int temp = -1;
	int i;
	for(i = 0; i < 30; i++)
	{
		if(strcmp(s,dic[i]) == 0)
			temp = i + 1;
	}
	return temp;
}

void code(char str[])
{
	char temp[2], P[50];
	int i = 1, j = 3;				//jΪĿǰ�ֵ�洢�����һ��λ��
	temp[0] = str[0];				//ȡ��һ���ַ�
	temp[1] = '\0';
	init();							//��ʼ��
	strcpy(P,temp);					//PΪǰ׺
	printf("����Ϊ��");
	while(1)
	{
		char t[2], C[50], PC[50], t2[50];
		t[0] = str[i];				//ȡ��һ�ַ�
		t[1] = '\0';
		strcpy(C,t);				//CΪ�ַ�������һ���ַ�
		strcpy(t2,P);				//t2�洢��ǰǰ׺
		if(strcmp(C,"") == 0)		//������Ҫ�룬����
		{
			printf("%5d",find(P));	//�������ǰǰ׺������
			break;					//�˳�ѭ��,�������
		}
		if(find(strcat(P,C)) > -1)	//������Ҫ�룬���P+C�ڴʵ��У�����C��չP��������һ����
		{							//ʹ��strcat()ʱ��P�ѱ�ΪP+C
			i++;
		}
		else						//���P+C���ڴʵ��У���P+C��ӵ��ʵ��У���P:=C
		{
			printf("%5d",find(t2));
			strcpy(PC,P);
			strcpy(dic[j++],PC);
			strcpy(P,C);
			i++;
		}
	}
	printf("\n���ɵĴʵ�Ϊ��\n");
	for(i = 0; i < j; i++)			//����ʵ��е�����,jΪ�ʵ�ĳ���
	{
		printf("%12d      ", i+1);
		puts(dic[i]);
	}
}
void main()							//������
{
	char str[50];
	printf("����Ҫ������ַ���(��a,b,c���)��");
	gets(str);
	code(str);
}