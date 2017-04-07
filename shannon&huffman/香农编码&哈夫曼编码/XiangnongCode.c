#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_BUFF	1000	//�ļ����������һǧ���ַ�
#define MAX_SHANNONCODE	30	//���ũ�볤

#define MAX_TREE	2*128-1	//n���ַ������ڵ���Ϊ2*n-1
#define MAX_HULFFCODE	30		//�������������볤
#define MAX_WEIGHT	1000	//�������Ȩֵ

char FileBuff[MAX_BUFF];

//��������
void CompressFile(int FileCnt , char Mod);

//��ũ����ṹ��
typedef struct ShannonCode
{
	char CharName;	//���ĵ��г��ֵ��ַ�
	unsigned int  AppearNum;	//���ִ���
	double	P;		//�ַ����ֵĸ���
	double	AddP;	//�ۼӸ���
	int CodeLength;	//�볤
	char ShannonCode[MAX_SHANNONCODE];	//��ũ��
}ShannonCodeTypedef;

//���������ṹ��
typedef struct HuffmanCode
{
	char CharName;
	int Weight;	//Ȩֵ(�ַ����ֵĴ���)
	int Parent;	//���׽ڵ��������еı��
	int LeftChild;	//����
	int RightChild;	//�Һ���
	char HulffCode[MAX_HULFFCODE];
}HuffmanTreeTypedef;

//���������ڵ�����
HuffmanTreeTypedef HulffmanTree[MAX_TREE];

ShannonCodeTypedef CharBuff[128];	//���128���ַ�

int CharCnt = 0;	//ͳ���ж��ٸ�����ͬ���ַ�
int FileCnt;	//�ļ��������ַ�����

//=====================================================
//		������:LoadCodeFile
//		  ����:���ĵ��е��ַ�������,�洢��FileBuff[]��
//		  ����:FileName	�ĵ���
//		  ����:�ĵ����ַ�������
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
		Cnt--;	//ȥ�����һ��������
	}
	else
	{
		printf("\n���ļ�ʧ��!\n");
		exit(1);
	}
	fclose(fp);
	return Cnt;
}

//================================================================
//		������:IsNewChar
//		  ����:�ж��Ƿ�Ϊ���ַ�
//		  ����:Character	Ҫ�жϵ��ַ�
//		  ����:	-1	Ϊ���ַ�
//				 i  ���������ַ����򷵻ظ��ַ���CharBuff�е����
//================================================================
int IsNewChar(char Character)
{
	int i;
	if(CharCnt == 0)	//��û���ַ�
	{
		return -1;
	}
	else
	{
		for(i = 0 ; i < CharCnt ; i++)
		{
			if(CharBuff[i].CharName == Character)
			{
				return i;	//�ҵ�ͬ�����ַ�
			}
		}
		return -1;	//���ַ�
	}
}

//================================================================
//		������:Statistics
//		  ����:ͳ���ַ����ִ��������洢�ַ�����
//		  ����:FileCnt	�ĵ����ַ�����
//		  ����:��
//================================================================
void Statistics(int FileCnt)
{
	int i;
	int CharNo;
	for(i = 0 ; i < FileCnt ; i++)
	{
		CharNo = IsNewChar(FileBuff[i]);
		if(CharNo < 0)	//��ʾΪ���ַ�
		{
			CharBuff[CharCnt].CharName = FileBuff[i];	//���ַ������ִ���CharBuff
			CharBuff[CharCnt].AppearNum = 1;			//���ַ���ʼ�����ִ���Ϊ1
			CharCnt++;									//��ͬ�����ַ��ĸ���++
		}
		else
		{
			CharBuff[CharNo].AppearNum++;				//���������ַ������ַ��ĳ��ִ���++
		}
	}
}

//================================================================
//		������:DispCharZoom
//		  ����:��ʾ��Դ�ռ�
//		  ����:FileCnt	�ĵ����ַ�����
//		  ����:��
//================================================================
void DispCharZoom(int FileCnt)
{
	int i;
	system("cls");	//����
	printf("���������Դ�ռ�Ϊ:\n");
	for(i = 0 ; i<CharCnt ; i++)
	{
		printf("����%d:%-5c����:%.4f\n" , i , CharBuff[i].CharName , CharBuff[i].P);
	}
	printf("���ַ���:%d\n" , FileCnt);
}

//================================================================
//		������:MoveChar
//		  ����:ð�ݷ�������Num����֮����С�����Ƶ�Numλ��
//		  ����:Num	��ǰ����ð�ݲ��������
//		  ����:��
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
//		������:SortChar
//		  ����:��ð�ݷ�����Դ�ռ�ĸ����ɴ�С��������
//		  ����:FileCnt	�ĵ����ַ�����
//		  ����:��
//================================================================
void SortChar(int FileCnt)	
{
	int i;
	for(i = 0 ; i<CharCnt ; i++)	//�����ַ�����
	{
		CharBuff[i].P = (float)(CharBuff[i].AppearNum)/FileCnt;	
	}
	for(i = 0 ; i<CharCnt ; i++)
	{
		MoveChar(i);	//����i�����������Ƶ�iλ��
	}
}

//================================================================
//		������:AddProbability
//		  ����:�����i���ַ�ǰ���ۼӸ���
//		  ����:��
//		  ����:��
//================================================================
void AddProbability(void)
{
	double AddPro = 0;	//�ۼӸ���
	int i , j;
	for(i = 0 ; i<CharCnt ; i++)
	{
		for(j = 0 ; j<i ; j++)
		{
			AddPro += CharBuff[j].P;	//�����i���ַ����ۼӸ���
		}
		CharBuff[i].AddP = AddPro;
		AddPro = 0;
	}
}

//========================================================================
//		������:CountCodeLength
//		  ����:������ũ���볤(-log(pi)<=Length<1-log(pi),����Ϊ���������)
//		  ����:��
//		  ����:��
//=========================================================================
void CountCodeLength(void)
{
	int i;
	for(i = 0 ; i<CharCnt ; i++)
	{
		//�����û��׹�ʽ��������Ϊ2
		//+0.999999Ϊ�˽�λ��Ҫ
		CharBuff[i].CodeLength = (int)(-log10(CharBuff[i].P)/log10(2) + 0.999999);
	}
}

//================================================================
//		������:CountShannonCode
//		  ����:������ũ��
//		  ����:��
//		  ����:��
//================================================================
void CountShannonCode(void)
{
	int i , j;
	double Temp;	//��ʱ�洢���ŵ��ۼӸ���
	for(i = 0 ; i<CharCnt ; i++)
	{
		Temp = CharBuff[i].AddP;
		for(j = 0 ; j<CharBuff[i].CodeLength ; j++)
		{
			Temp = Temp*2;	//���ۼӸ��ʳ�2��������1�����1��ͬʱֻȡС��
			if(Temp >= 1)
			{
				CharBuff[i].ShannonCode[j] = '1';	//��λ���1
				Temp -= 1;	//ȥ���������֣�ֻȡС��
			}
			else
			{
				CharBuff[i].ShannonCode[j] = '0';	//�����0
			}		
		}
		CharBuff[i].ShannonCode[j+1] = '\0';	//���Ͻ�����
	}
}

//================================================================
//		������:DispShannonCode
//		  ����:��ʾ��ũ��
//		  ����:��
//		  ����:��
//================================================================
void DispShannonCode(void)
{
	int i;
	printf("\n��ũ�������:\n");
	for(i = 0 ; i<CharCnt ; i++)
	{
		printf("����%d:%-5c��ũ��:%s\n" , i , CharBuff[i].CharName , CharBuff[i].ShannonCode);
	}
}


//================================================================
//		������:HulffTreeInit
//		  ����:�������ṹ��ʼ�������׽ڵ�Ϊ-1��ʾ�޸��׽ڵ�
//		  ����:��
//		  ����:��
//================================================================
void HulffTreeInit(void)
{
	int i;
	for(i = 0 ; i<MAX_TREE ; i++)
	{
		if(i<CharCnt)
		{
			HulffmanTree[i].CharName = CharBuff[i].CharName;	//���ַ����ݴ�CharBuff�Ƶ�HulffmanTree��
			HulffmanTree[i].Weight = CharBuff[i].AppearNum;
		}
		HulffmanTree[i].Parent = -1;	//�洢���׻����������е����,Ϊ-1��ʾ��
		HulffmanTree[i].LeftChild = -1;
		HulffmanTree[i].RightChild = -1;
	}
}

//================================================================
//		������:MakeHuffmanTree
//		  ����:�����������
//		  ����:��
//		  ����:��
//================================================================
void MakeHuffmanTree(void)
{
	int i , j;
	int weight1 , weight2;	//������С������Ȩֵ
	int leftNum , rightNum;	//�������Һ��ӵ����
	weight1 = weight2 = MAX_WEIGHT;	//��Ȩֵ��һ���ܴ�ĳ�ʼֵ���������ıȽ�

	for(i = 0 ; i<CharCnt-1 ; i++)
	{
		for(j = 0 ; j<CharCnt+i ; j++)
		{
			//���������С��Ȩֵ�ڵ�
			if(HulffmanTree[j].Parent == -1 && HulffmanTree[j].Weight < weight1)	//�����С��Ȩֵ��
			{
				weight2 = weight1;	//���ַ��ָ�С�ģ����Ƚ�Ŀǰ�����Ȩֵ��Ϣ����weight2
				rightNum = leftNum;

				weight1 = HulffmanTree[j].Weight;	//����Ȩֵ
				leftNum = j;	//�������ӵ����
			}
			else if(HulffmanTree[j].Parent == -1 && HulffmanTree[j].Weight < weight2)	//��ڶ�С��Ȩֵ��
			{
				weight2 = HulffmanTree[j].Weight;	//����Ȩֵ
				rightNum = j;	//�����Һ��ӵ����				
			}
		}
		HulffmanTree[CharCnt+i].Weight = HulffmanTree[leftNum].Weight + HulffmanTree[rightNum].Weight;	//���㸸�׽ڵ��Ȩֵ
		HulffmanTree[leftNum].Parent = HulffmanTree[rightNum].Parent = CharCnt+i;	//�����׽ڵ�ı�Ÿ������ӽڵ�
		HulffmanTree[CharCnt+i].LeftChild = leftNum;	//�����Һ��ӵ���Ÿ������׽ڵ�
		HulffmanTree[CharCnt+i].RightChild = rightNum;
		weight1 = weight2 = MAX_WEIGHT;	//���¸���ʼֵ
	}
}

//================================================================
//		������:MakeHulffmanCode
//		  ����:�������������
//		  ����:��
//		  ����:��
//================================================================
void MakeHulffmanCode(void)
{
	int i , j;
	int parent;	//�����洢Ŀǰ�����ڵ�ĸ���
	int Newchild;	//�洢Ŀǰ�����ĺ��ӽڵ�
	char CodeBuff[MAX_HULFFCODE];	//�洢���ɵĹ������룬ע�⣬�������ʵ�Ĺ��������Ƿ��ģ��ʻ���Ҫ����һ����������
	int Cnt;	//�洢��ǰҶ�ӽڵ����ɵĹ���������볤

	for(i = 0 ; i<CharCnt ; i++)
	{
		Cnt = 0;
		parent = HulffmanTree[i].Parent;	//���µ�ǰ�����ڵ�ĸ��׽ڵ�
		Newchild = i;	//���µ�ǰ�������ӽڵ�
		while(HulffmanTree[Newchild].Parent != -1)	//����ǰ�ĺ��ӽڵ��и��׽ڵ�
		{
			if(HulffmanTree[parent].LeftChild == Newchild)	//�������� ���Ϊ0
			{
				CodeBuff[Cnt] = '0';
				Cnt++;
			}
			else if(HulffmanTree[parent].RightChild == Newchild)	//�����Һ��� ���Ϊ1
			{
				CodeBuff[Cnt] = '1';
				Cnt++;
			}
			Newchild = parent;	//�����׽ڵ��λ���ӽڵ㣬�ٽ����ж��Ƿ��и��׽ڵ㣬��û���ˣ����ʾ�õ��Ѿ��Ǹ��ڵ㣬�������
			parent = HulffmanTree[Newchild].Parent;
		}
		for(j = 0 ; j<Cnt ; j++)
		{
			HulffmanTree[i].HulffCode[j] = CodeBuff[Cnt-1-j];	//���з���������õ�������Ҫ�Ĺ�������
		}
		HulffmanTree[i].HulffCode[Cnt] = '\0';	//���ٽ�����
	}
}

//================================================================
//		������:DispHufmanCode
//		  ����:��ʾ��������
//		  ����:��
//		  ����:��
//================================================================
void DispHufmanCode(void)
{
	int i;
	printf("\n�������������:\n");
	for(i = 0 ; i<CharCnt ; i++)
	{
		printf("����%d:%-5c��������:%s\n" , i , HulffmanTree[i].CharName , HulffmanTree[i].HulffCode);
	}
}

//================================================================
//		������:Selec
//		  ����:ѡ���ǽ�����ũ���뻹�ǹ���������
//		  ����:��
//		  ����:��
//================================================================
void Select(void)
{
	char Choise;
	char Ch;
	printf("\n��ѡ����뷽ʽ");
	printf("\n1.��ũ��		2.��������\n");
	Choise = getch();
	if(Choise == '1')	//��ũ������
	{
		AddProbability();	//�����ۼӸ���
		CountCodeLength();	//�������ֵ��볤
		CountShannonCode();
		DispShannonCode(); 
	}
	else if(Choise == '2')	//����������
	{
		HulffTreeInit();
		MakeHuffmanTree();
		MakeHulffmanCode();
		DispHufmanCode();
	}
	Ch = getch();
	printf("\n�Ƿ�����ѹ���ļ�?	Y/N\n");
	
	if(Ch == 'Y' || Ch == 'y')	//����ѹ���ļ�
	{
		CompressFile(FileCnt , Choise);
	}
	
}

//==================================================================
//		������:CountCharNum
//		  ����:��������ַ�������������ũ�����������������еı��
//		  ����:Ch ������ַ�	
//			   Mod '1' ��ũ����		'2'����������
//		  ����:�������еı���
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
		return -1;	//δ�ҵ�
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
		return -1;	//δ�ҵ�
	}
}

//================================================================
//		������:CompressFile
//		  ����:����ѹ���ļ�
//		  ����:FileCnt	�ĵ����ַ�����
//			   Mod	'1'��ũ����		'2'����������
//		  ����:��
//================================================================
void CompressFile(int FileCnt , char Mod)
{
	if(Mod == '1')	//��ũ��ѹ��
	{
		FILE *fp;
		
		int i = 1;	//�Ѿ�ѹ�����ַ�����
		int ChNum;	//��ǰ�������ַ��������еı��
		unsigned int TotalBit = 0;	//д���ļ�����λ��
		unsigned int BitCnt = 0;	//����������ÿд���λ����һ���ֽں󣬾ͽ�����д���ļ�
		unsigned char WriteByte = 0;	//Ҫд���ļ�������(8λ)
		
		char* Ch_P;	//ָ���ַ��ı�������
		char FinishFlag = 0;	//ѹ����ɱ�־
		
		fp = fopen("ShannonCompress.bin" , "wb");
		
		ChNum = CountCharNum(FileBuff[0] , Mod);	//��ʼ����ֵ
		Ch_P = CharBuff[ChNum].ShannonCode;
		
		while(1)
		{
			while(BitCnt < 8)	//�ж��Ƿ�д��һ���ֽ�
			{
				WriteByte |= ((*Ch_P-30)&0x01)<<(7-BitCnt);	//һλһλ�ĸ�ֵ
				Ch_P++;	//ָ������������һ����
				if(*Ch_P == '\0')	//�ж�ĳ�����ŵ���ֵ���Ѹ���
				{
					ChNum = CountCharNum(FileBuff[i] , Mod);	//ĳ�����ָ��꣬�򽫽��ĵ��е���һ���ַ�����ȡ�����������и�ֵ
					Ch_P = CharBuff[ChNum].ShannonCode;
					i++;	//�Ѿ�ѹ�����ַ���++
					if(FileBuff[i] == '\0')		//�ж��Ƿ�ѹ�����
					{
						FinishFlag = 1;	//�������Ϊ��ɱ�־
						break;	
					}
				}
				BitCnt++;	
				TotalBit++;
			}
			BitCnt = 0; //д��һ���ֽڣ������ֽڼ���
			fputc(WriteByte , fp);	//���ļ���д�븳ֵ����ֽ�
			WriteByte = 0;	//�����д����ֽ����ݣ�������һ�εļ�����ֵ
			if(FinishFlag)	//�ж�ѹ���Ƿ����
			{
				fp = fopen("ShannonCompress.txt" , "wt");
				for(i = 0 ; i<CharCnt ; i++)	//����ɣ��򽫶�Ӧ�����Լ����ŵı����д�����λ��д��һ���ļ����������
				{
					fprintf(fp , "%c %s\n" , CharBuff[i].CharName , CharBuff[i].ShannonCode);
				}
				fprintf(fp , "%d" , TotalBit+1);	//TotalBit�Ǵ�0��ʼ������
				fclose(fp);	//�ر��ļ�ָ��
				return ;
			}
		}
	}

	if(Mod == '2')	//��������ѹ��
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
//		������:main
//		  ����:������
//		  ����:��
//		  ����:��
//================================================================
void main(void)
{
		FileCnt = 0;							//���ַ���
		FileCnt = LoadCodeFile("bianma.txt");	//Ҫ����ѹ�����ļ���
		Statistics(FileCnt);					//�ַ�ͳ��
		SortChar(FileCnt);						//��������,�ɴ�С
		DispCharZoom(FileCnt);					//��ʾ��Դ�ռ�
		Select();
}