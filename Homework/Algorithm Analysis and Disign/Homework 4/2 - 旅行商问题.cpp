#include <iostream>
#include <cstdbool>
#include <cstdio>
#include <cmath>
#include <ctime>
#include "libxl.h" //���ڶ�ȡexcel�ļ�

using namespace libxl; //���ڶ�ȡexcel�ļ�
using namespace std;

#define NUM1 22 //��һ���ļ���22����վ���ڽӾ���

double arc[50][50];
int ans[50];
int bestAns[50];
double value;
double bestValue;
int nodeNum;

void tsp(int num);
void backtrack(int i, int num);


//���ݺ��ļ�����
int readData(Book* book, double arc[][50], wchar_t loadFileName[], int sheetNum, int n);

int main(void){
	//��excel�ļ��ж�ȡ���� 
	Book* book = xlCreateBook();
	if (!book){
		cout << "Error when init book." << endl;
		return -1;
	}

	//��ȡ����
	wchar_t loadFileName1[] = L"����1-1.��վͼ���ڽӾ���-v1.xls";
	if (readData(book, arc, loadFileName1, 1, NUM1) <= 0)
		return -2;
	
	//��ʱ
	clock_t start, finish;
	double duration;

	start = clock();
	tsp(NUM1);
	finish = clock();
	duration = (double)(finish - start);
	
	//������
	cout << "The shortest length is " << bestValue << endl;
	cout << "The time cost is " << duration << endl;
	cout<< "The node travelled: " << nodeNum << endl;
	cout << "The road is: ";
	for (int i = 1; i <= NUM1; i++){
		cout << bestAns[i] << "->";
	}
	cout << endl;

	system("PAUSE");
	return 0;
}

void tsp(int num){ //��ʼ����������ʼ���ݹ���
	for (int i = 1; i <= num; i++)
		ans[i] = i;
	bestValue = 10000;
	value = 0;
	nodeNum = 0;
	backtrack(2,num);
}

void backtrack(int i,int num){ //���ݱ��������ڵ�
	nodeNum++;
	int temp;
	if (i == num){ //�����������������������Ϊ���Ž��
		if (arc[ans[num - 1]][ans[num]] < 10000 &&
			arc[ans[num]][1] < 10000 &&
			(bestValue == 10000 || value + arc[ans[num - 1]][ans[num]] + arc[ans[num]][1] < bestValue)){
			for (int j = 1; j <= num; j++)
				bestAns[j] = ans[j];
			bestValue = value + arc[ans[num - 1]][ans[num]] + arc[ans[num]][1];
		}
	}
	else{ //�������������������i��j����һ���ڵ����
		for (int j = i; j <= num;j++)
			if (arc[ans[i - 1]][ans[j]] < 10000 && (bestValue == 10000 || value + arc[ans[i - 1]][ans[j]] < bestValue)){
				temp = ans[i];
				ans[i] = ans[j];
				ans[j] = temp;
				
				value += arc[ans[i - 1]][ans[i]];
				backtrack(i + 1,num);
				value -= arc[ans[i - 1]][ans[i]];
				
				temp = ans[i];
				ans[i] = ans[j];
				ans[j] = temp;
			}
	}
}

//�������ļ�����
int readData(Book* book, double arc[][50], wchar_t loadFileName[], int sheetNum, int n){ //�����ݴ�excel�ļ��ж���
	if (book->load(loadFileName)){ //��ȡbook
		cout << "�Ѷ�ȡ�ļ���" << endl;
	}
	else{
		cout << "��ȡ�ļ�ʱ����" << endl;
		return -2;
	}
	Sheet* sheet = book->getSheet(sheetNum);//��ȡexcel�ļ��е�sheet2 
	if (sheet){ //��sheet�е����ݸ��Ƶ��ṹ������
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++){
				arc[i+1][j+1] = sheet->readNum(i + 2, j + 2);
				if (arc[i+1][j+1] == -1)
					arc[i+1][j+1] = 10000;
			}
	}
	book->release();
	return 1;
}
