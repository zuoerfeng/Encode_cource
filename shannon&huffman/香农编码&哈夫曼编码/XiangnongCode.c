#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_BUFF	1000	//文件缓存区最多一千个字符
#define MAX_SHANNONCODE	30	//最长香农码长

#define MAX_TREE	2*128-1	//n个字符的最大节点数为2*n-1
#define MAX_HULFFCODE	30		//定义最大哈夫曼码长
#define MAX_WEIGHT	1000	//定义最大权值

char FileBuff[MAX_BUFF];

//函数声明
void CompressFile(int FileCnt , char Mod);

//香农编码结构体
typedef struct ShannonCode
{
	char CharName;	//在文档中出现的字符
	unsigned int  AppearNum;	//出现次数
	double	P;		//字符出现的概率
	double	AddP;	//累加概率
	int CodeLength;	//码长
	char ShannonCode[MAX_SHANNONCODE];	//香农码
}ShannonCodeTypedef;

//哈夫曼树结构体
typedef struct HuffmanCode
{
	char CharName;
	int Weight;	//权值(字符出现的次数)
	int Parent;	//父亲节点在数组中的标号
	int LeftChild;	//左孩子
	int RightChild;	//右孩子
	char HulffCode[MAX_HULFFCODE];
}HuffmanTreeTypedef;

//哈夫曼树节点数组
HuffmanTreeTypedef HulffmanTree[MAX_TREE];

ShannonCodeTypedef CharBuff[128];	//最多128个字符

int CharCnt = 0;	//统计有多少个不相同的字符
int FileCnt;	//文件中所含字符总数

//=====================================================
//		函数名:LoadCodeFile
//		  功能:将文档中的字符串读入,存储在FileBuff[]中
//		  参数:FileName	文档名
//		  返回:文档中字符的总数
//=====================================================
int LoadCodeFile(char *FileName)
{
	FILE *fp;
	int Cnt = 0;
	fp = fopen(FileName , "rt");
	if(fp != NULL)
	{
		while(!feof(fp))
		{
			FileBuff[Cnt] = fgetc(fp);
			Cnt++;
		}
		Cnt--;	//去掉最后一个结束符
	}
	else
	{
		printf("\n打开文件失败!\n");
		exit(1);
	}
	fclose(fp);
	return Cnt;
}

//================================================================
//		函数名:IsNewChar
//		  功能:判断是否为新字符
//		  参数:Character	要判断的字符
//		  返回:	-1	为新字符
//				 i  若不是新字符，则返回该字符在CharBuff中的序号
//================================================================
int IsNewChar(char Character)
{
	int i;
	if(CharCnt == 0)	//还没有字符
	{
		return -1;
	}
	else
	{
		for(i = 0 ; i < CharCnt ; i++)
		{
			if(CharBuff[i].CharName == Character)
			{
				return i;	//找到同样的字符
			}
		}
		return -1;	//新字符
	}
}

//================================================================
//		函数名:Statistics
//		  功能:统计字符出现次数，并存储字符名称
//		  参数:FileCnt	文档中字符总数
//		  返回:无
//================================================================
void Statistics(int FileCnt)
{
	int i;
	int CharNo;
	for(i = 0 ; i < FileCnt ; i++)
	{
		CharNo = IsNewChar(FileBuff[i]);
		if(CharNo < 0)	//表示为新字符
		{
			CharBuff[CharCnt].CharName = FileBuff[i];	//将字符的名字存入CharBuff
			CharBuff[CharCnt].AppearNum = 1;			//新字符初始化出现次数为1
			CharCnt++;									//不同种类字符的个数++
		}
		else
		{
			CharBuff[CharNo].AppearNum++;				//若不是新字符，则将字符的出现次数++
		}
	}
}

//================================================================
//		函数名:DispCharZoom
//		  功能:显示信源空间
//		  参数:FileCnt	文档中字符总数
//		  返回:无
//================================================================
void DispCharZoom(int FileCnt)
{
	int i;
	system("cls");	//清屏
	printf("您输入的信源空间为:\n");
	for(i = 0 ; i<CharCnt ; i++)
	{
		printf("符号%d:%-5c概率:%.4f\n" , i , CharBuff[i].CharName , CharBuff[i].P);
	}
	printf("总字符数:%d\n" , FileCnt);
}

//================================================================
//		函数名:MoveChar
//		  功能:冒泡法，将第Num个数之后最小的数移到Num位置
//		  参数:Num	当前进行冒泡操作的序号
//		  返回:无
//================================================================
void MoveChar(int Num)
{
	int i;
	ShannonCodeTypedef Temp;
	for(i = Num ; i<CharCnt ; i++)
	{
		if(CharBuff[i].P > CharBuff[Num].P)
		{
			Temp.CharName = CharBuff[Num].CharName;
			Temp.AppearNum = CharBuff[Num].AppearNum;
			Temp.P = CharBuff[Num].P;
			CharBuff[Num].CharName = CharBuff[i].CharName;
			CharBuff[Num].AppearNum = CharBuff[i].AppearNum;
			CharBuff[Num].P = CharBuff[i].P;
			CharBuff[i].CharName = Temp.CharName;
			CharBuff[i].AppearNum = Temp.AppearNum;
			CharBuff[i].P = Temp.P;
		}
	}
}

//================================================================
//		函数名:SortChar
//		  功能:用冒泡法对信源空间的概率由大到小进行排序
//		  参数:FileCnt	文档中字符总数
//		  返回:无
//================================================================
void SortChar(int FileCnt)	
{
	int i;
	for(i = 0 ; i<CharCnt ; i++)	//计算字符概率
	{
		CharBuff[i].P = (float)(CharBuff[i].AppearNum)/FileCnt;	
	}
	for(i = 0 ; i<CharCnt ; i++)
	{
		MoveChar(i);	//将第i个后最大的数移到i位置
	}
}

//================================================================
//		函数名:AddProbability
//		  功能:计算第i个字符前的累加概率
//		  参数:无
//		  返回:无
//================================================================
void AddProbability(void)
{
	double AddPro = 0;	//累加概率
	int i , j;
	for(i = 0 ; i<CharCnt ; i++)
	{
		for(j = 0 ; j<i ; j++)
		{
			AddPro += CharBuff[j].P;	//计算第i个字符的累加概率
		}
		CharBuff[i].AddP = AddPro;
		AddPro = 0;
	}
}

//========================================================================
//		函数名:CountCodeLength
//		  功能:计算香农码码长(-log(pi)<=Length<1-log(pi),底数为编码进制数)
//		  参数:无
//		  返回:无
//=========================================================================
void CountCodeLength(void)
{
	int i;
	for(i = 0 ; i<CharCnt ; i++)
	{
		//首先用换底公式将底数换为2
		//+0.999999为了进位需要
		CharBuff[i].CodeLength = (int)(-log10(CharBuff[i].P)/log10(2) + 0.999999);
	}
}

//================================================================
//		函数名:CountShannonCode
//		  功能:计算香农码
//		  参数:无
//		  返回:无
//================================================================
void CountShannonCode(void)
{
	int i , j;
	double Temp;	//暂时存储符号的累加概率
	for(i = 0 ; i<CharCnt ; i++)
	{
		Temp = CharBuff[i].AddP;
		for(j = 0 ; j<CharBuff[i].CodeLength ; j++)
		{
			Temp = Temp*2;	//将累加概率乘2，若大于1，则编1，同时只取小数
			if(Temp >= 1)
			{
				CharBuff[i].ShannonCode[j] = '1';	//进位则编1
				Temp -= 1;	//去掉整数部分，只取小数
			}
			else
			{
				CharBuff[i].ShannonCode[j] = '0';	//否则编0
			}		
		}
		CharBuff[i].ShannonCode[j+1] = '\0';	//加上结束符
	}
}

//================================================================
//		函数名:DispShannonCode
//		  功能:显示香农码
//		  参数:无
//		  返回:无
//================================================================
void DispShannonCode(void)
{
	int i;
	printf("\n香农码编码结果:\n");
	for(i = 0 ; i<CharCnt ; i++)
	{
		printf("符号%d:%-5c香农码:%s\n" , i , CharBuff[i].CharName , CharBuff[i].ShannonCode);
	}
}


//================================================================
//		函数名:HulffTreeInit
//		  功能:哈夫曼结构初始化，父亲节点为-1表示无父亲节点
//		  参数:无
//		  返回:无
//================================================================
void HulffTreeInit(void)
{
	int i;
	for(i = 0 ; i<MAX_TREE ; i++)
	{
		if(i<CharCnt)
		{
			HulffmanTree[i].CharName = CharBuff[i].CharName;	//将字符数据从CharBuff移到HulffmanTree中
			HulffmanTree[i].Weight = CharBuff[i].AppearNum;
		}
		HulffmanTree[i].Parent = -1;	//存储父亲或孩子在数组中的序号,为-1表示无
		HulffmanTree[i].LeftChild = -1;
		HulffmanTree[i].RightChild = -1;
	}
}

//================================================================
//		函数名:MakeHuffmanTree
//		  功能:构造哈夫曼树
//		  参数:无
//		  返回:无
//================================================================
void MakeHuffmanTree(void)
{
	int i , j;
	int weight1 , weight2;	//保存最小的两个权值
	int leftNum , rightNum;	//保存左右孩子的序号
	weight1 = weight2 = MAX_WEIGHT;	//给权值赋一个很大的初始值，方便后面的比较

	for(i = 0 ; i<CharCnt-1 ; i++)
	{
		for(j = 0 ; j<CharCnt+i ; j++)
		{
			//求出两个最小的权值节点
			if(HulffmanTree[j].Parent == -1 && HulffmanTree[j].Weight < weight1)	//求出最小的权值点
			{
				weight2 = weight1;	//若又发现更小的，则先将目前保存的权值信息赋给weight2
				rightNum = leftNum;

				weight1 = HulffmanTree[j].Weight;	//保存权值
				leftNum = j;	//保存左孩子的序号
			}
			else if(HulffmanTree[j].Parent == -1 && HulffmanTree[j].Weight < weight2)	//求第二小的权值点
			{
				weight2 = HulffmanTree[j].Weight;	//保存权值
				rightNum = j;	//保存右孩子的序号				
			}
		}
		HulffmanTree[CharCnt+i].Weight = HulffmanTree[leftNum].Weight + HulffmanTree[rightNum].Weight;	//计算父亲节点的权值
		HulffmanTree[leftNum].Parent = HulffmanTree[rightNum].Parent = CharCnt+i;	//将父亲节点的编号赋给孩子节点
		HulffmanTree[CharCnt+i].LeftChild = leftNum;	//将左、右孩子的序号赋给父亲节点
		HulffmanTree[CharCnt+i].RightChild = rightNum;
		weight1 = weight2 = MAX_WEIGHT;	//重新赋初始值
	}
}

//================================================================
//		函数名:MakeHulffmanCode
//		  功能:计算哈夫曼编码
//		  参数:无
//		  返回:无
//================================================================
void MakeHulffmanCode(void)
{
	int i , j;
	int parent;	//用来存储目前操作节点的父亲
	int Newchild;	//存储目前操作的孩子节点
	char CodeBuff[MAX_HULFFCODE];	//存储生成的哈夫曼码，注意，这里和真实的哈夫曼码是反的，故还需要进行一个反向排序
	int Cnt;	//存储当前叶子节点生成的哈夫曼码的码长

	for(i = 0 ; i<CharCnt ; i++)
	{
		Cnt = 0;
		parent = HulffmanTree[i].Parent;	//更新当前操作节点的父亲节点
		Newchild = i;	//更新当前操作孩子节点
		while(HulffmanTree[Newchild].Parent != -1)	//若当前的孩子节点有父亲节点
		{
			if(HulffmanTree[parent].LeftChild == Newchild)	//若是左孩子 则编为0
			{
				CodeBuff[Cnt] = '0';
				Cnt++;
			}
			else if(HulffmanTree[parent].RightChild == Newchild)	//若是右孩子 则编为1
			{
				CodeBuff[Cnt] = '1';
				Cnt++;
			}
			Newchild = parent;	//将父亲节点该位孩子节点，再进行判断是否有父亲节点，若没有了，则表示该点已经是根节点，编码完成
			parent = HulffmanTree[Newchild].Parent;
		}
		for(j = 0 ; j<Cnt ; j++)
		{
			HulffmanTree[i].HulffCode[j] = CodeBuff[Cnt-1-j];	//进行反向排序，则得到我们需要的哈夫曼码
		}
		HulffmanTree[i].HulffCode[Cnt] = '\0';	//加速结束符
	}
}

//================================================================
//		函数名:DispHufmanCode
//		  功能:显示哈夫曼码
//		  参数:无
//		  返回:无
//================================================================
void DispHufmanCode(void)
{
	int i;
	printf("\n哈夫曼码编码结果:\n");
	for(i = 0 ; i<CharCnt ; i++)
	{
		printf("符号%d:%-5c哈夫曼码:%s\n" , i , HulffmanTree[i].CharName , HulffmanTree[i].HulffCode);
	}
}

//================================================================
//		函数名:Selec
//		  功能:选择是进行香农编码还是哈夫曼编码
//		  参数:无
//		  返回:无
//================================================================
void Select(void)
{
	char Choise;
	char Ch;
	printf("\n请选择编码方式");
	printf("\n1.香农码		2.哈夫曼码\n");
	Choise = getch();
	if(Choise == '1')	//香农比那吗
	{
		AddProbability();	//计算累加概率
		CountCodeLength();	//计算码字的码长
		CountShannonCode();
		DispShannonCode(); 
	}
	else if(Choise == '2')	//哈夫曼编码
	{
		HulffTreeInit();
		MakeHuffmanTree();
		MakeHulffmanCode();
		DispHufmanCode();
	}
	Ch = getch();
	printf("\n是否生成压缩文件?	Y/N\n");
	
	if(Ch == 'Y' || Ch == 'y')	//生成压缩文件
	{
		CompressFile(FileCnt , Choise);
	}
	
}

//==================================================================
//		函数名:CountCharNum
//		  功能:由输入的字符，返回其在香农码数组或哈夫曼数组中的编号
//		  参数:Ch 输入的字符	
//			   Mod '1' 香农编码		'2'哈夫曼编码
//		  返回:在数组中的编码
//==================================================================
int CountCharNum(char Ch , char Mod)
{
	if(Mod == '1')
	{
		int i;
		for(i = 0 ; i<CharCnt ; i++)
		{
			if(Ch == CharBuff[i].CharName)
			{
				return i;
			}
		}
		return -1;	//未找到
	}
	else
	{
		int i;
		for(i = 0 ; i<CharCnt ; i++)
		{
			if(Ch == HulffmanTree[i].CharName)
			{
				return i;
			}
		}
		return -1;	//未找到
	}
}

//================================================================
//		函数名:CompressFile
//		  功能:生成压缩文件
//		  参数:FileCnt	文档中字符总数
//			   Mod	'1'香农编码		'2'哈夫曼编码
//		  返回:无
//================================================================
void CompressFile(int FileCnt , char Mod)
{
	if(Mod == '1')	//香农码压缩
	{
		FILE *fp;
		
		int i = 1;	//已经压缩的字符个数
		int ChNum;	//当前操作的字符在数组中的编号
		unsigned int TotalBit = 0;	//写入文件的总位数
		unsigned int BitCnt = 0;	//用来计数，每写入八位，即一个字节后，就将数据写入文件
		unsigned char WriteByte = 0;	//要写入文件的数据(8位)
		
		char* Ch_P;	//指向字符的编码数组
		char FinishFlag = 0;	//压缩完成标志
		
		fp = fopen("ShannonCompress.bin" , "wb");
		
		ChNum = CountCharNum(FileBuff[0] , Mod);	//初始化赋值
		Ch_P = CharBuff[ChNum].ShannonCode;
		
		while(1)
		{
			while(BitCnt < 8)	//判断是否写完一个字节
			{
				WriteByte |= ((*Ch_P-30)&0x01)<<(7-BitCnt);	//一位一位的赋值
				Ch_P++;	//指向编码数组的下一个码
				if(*Ch_P == '\0')	//判断某个符号的码值都已赋完
				{
					ChNum = CountCharNum(FileBuff[i] , Mod);	//某个码字赋完，则将将文档中的下一个字符的码取出，继续进行赋值
					Ch_P = CharBuff[ChNum].ShannonCode;
					i++;	//已经压缩的字符数++
					if(FileBuff[i] == '\0')		//判断是否压缩完成
					{
						FinishFlag = 1;	//完成则置为完成标志
						break;	
					}
				}
				BitCnt++;	
				TotalBit++;
			}
			BitCnt = 0; //写完一个字节，清零字节计数
			fputc(WriteByte , fp);	//向文件中写入赋值后的字节
			WriteByte = 0;	//清零待写入的字节数据，方便下一次的继续赋值
			if(FinishFlag)	//判断压缩是否完成
			{
				fp = fopen("ShannonCompress.txt" , "wt");
				for(i = 0 ; i<CharCnt ; i++)	//若完成，则将对应符号以及符号的编码和写入的总位数写入一个文件，方便解码
				{
					fprintf(fp , "%c %s\n" , CharBuff[i].CharName , CharBuff[i].ShannonCode);
				}
				fprintf(fp , "%d" , TotalBit+1);	//TotalBit是从0开始计数的
				fclose(fp);	//关闭文件指针
				return ;
			}
		}
	}

	if(Mod == '2')	//哈夫曼码压缩
	{
		FILE *fp;
		
		int i = 1;
		int ChNum;
		unsigned int TotalBit = 0;
		unsigned int BitCnt = 0;
		unsigned char WriteByte = 0;
		
		char* Ch_P;
		char FinishFlag = 0;
		
		fp = fopen("HuffmanCompress.bin" , "wb");
		
		ChNum = CountCharNum(FileBuff[0] , Mod);
		Ch_P = HulffmanTree[ChNum].HulffCode;
		
		while(1)
		{
			while(BitCnt < 8)
			{
				WriteByte |= ((*Ch_P-30)&0x01)<<(7-BitCnt);
				Ch_P++;
				if(*Ch_P == '\0')
				{
					ChNum = CountCharNum(FileBuff[i] , Mod);
					Ch_P = HulffmanTree[ChNum].HulffCode;
					i++;
					if(FileBuff[i] == '\0')
					{
						FinishFlag = 1;
						break;
					}
				}
				BitCnt++;
				TotalBit++;
			}
			BitCnt = 0;
			fputc(WriteByte , fp);
			WriteByte = 0;
			if(FinishFlag)
			{
				fp = fopen("HuffmanCompress.txt" , "wt");
				for(i = 0 ; i<CharCnt ; i++)
				{
					fprintf(fp , "%c %s\n" , HulffmanTree[i].CharName , HulffmanTree[i].HulffCode);
				}
				fprintf(fp , "%d" , TotalBit+1);
				fclose(fp);
				return ;
			}
		}
	}
}

//================================================================
//		函数名:main
//		  功能:主函数
//		  参数:无
//		  返回:无
//================================================================
void main(void)
{
		FileCnt = 0;							//总字符数
		FileCnt = LoadCodeFile("bianma.txt");	//要进行压缩的文件名
		Statistics(FileCnt);					//字符统计
		SortChar(FileCnt);						//概率排序,由大到小
		DispCharZoom(FileCnt);					//显示信源空间
		Select();
}