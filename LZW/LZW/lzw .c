#include<stdio.h>
#include<string.h>

char dic[30][50];
int n;

void init()							//字典初始化
{
	int i;
	strcpy(dic[0],"a");				//开始时词典包含所有可能的根
	strcpy(dic[1],"b");
	strcpy(dic[2],"c");
	for(i = 3; i < 30; i++)			//其余为空
		strcpy(dic[i],"");
}

int find(char s[])					//字典中寻找,返回序号
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
	int i = 1, j = 3;				//j为目前字典存储的最后一个位置
	temp[0] = str[0];				//取第一个字符
	temp[1] = '\0';
	init();							//初始化
	strcpy(P,temp);					//P为前缀
	printf("编码为：");
	while(1)
	{
		char t[2], C[50], PC[50], t2[50];
		t[0] = str[i];				//取下一字符
		t[1] = '\0';
		strcpy(C,t);				//C为字符流中下一个字符
		strcpy(t2,P);				//t2存储当前前缀
		if(strcmp(C,"") == 0)		//无码字要译，结束
		{
			printf("%5d",find(P));	//输出代表当前前缀的码字
			break;					//退出循环,编码结束
		}
		if(find(strcat(P,C)) > -1)	//有码字要译，如果P+C在词典中，则用C扩展P，进行下一步：
		{							//使用strcat()时，P已变为P+C
			i++;
		}
		else						//如果P+C不在词典中，则将P+C添加到词典中，令P:=C
		{
			printf("%5d",find(t2));
			strcpy(PC,P);
			strcpy(dic[j++],PC);
			strcpy(P,C);
			i++;
		}
	}
	printf("\n生成的词典为：\n");
	for(i = 0; i < j; i++)			//输出词典中的内容,j为词典的长度
	{
		printf("%12d      ", i+1);
		puts(dic[i]);
	}
}
void main()							//主程序
{
	char str[50];
	printf("输入要编码的字符串(由a,b,c组成)：");
	gets(str);
	code(str);
}