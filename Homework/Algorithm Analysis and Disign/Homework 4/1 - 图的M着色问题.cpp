#include <iostream>
#include <cstdbool>
#include <cstdio>
#include <cmath>
#include <ctime>
#include "libxl.h" //���ڶ�ȡexcel�ļ�

using namespace libxl; //���ڶ�ȡexcel�ļ�
using namespace std;

#define NUM1 22 //��һ���ļ���22����վ���ڽӾ���
#define NUM2 42

int colorNum;
int arc[50][50];
int ans[50];
long int sum;
long long int nodeNum;
bool isFirst;

void backtrack(int t, int num);
void m_coloring(int num);
bool ok(int k);
//���ݺ��ļ�����
int readData(Book* book, int arc[][50], wchar_t loadFileName[], int sheetNum, int n);

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
	
	clock_t start, finish;
	double duration;

	for (colorNum = 1; colorNum <= NUM1; colorNum++){
		isFirst = true;
		sum = 0;
		nodeNum = 0;
		start = clock();
		backtrack(1, NUM1);
		finish = clock();
		duration = (double)(finish - start);
		if (sum != 0){
			cout << "The minimum number of color is " << colorNum << endl;
			cout << "The sum is " << sum << ".And the time is " << duration << endl;
			cout << "The nodes travelled:" << nodeNum << endl << endl;
			break;
		}
		else{
			cout << colorNum << ':' << "No Ans." << endl;
			cout << "The time is " << duration << endl;
		}
	}

	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	if (readData(book2, arc, loadFileName1, 0, NUM2) <= 0)
		return -2;

	for (colorNum = 1; colorNum <= NUM2; colorNum++){
		sum = 0;
		nodeNum = 0;
		start = clock();
		backtrack(1, NUM2);
		finish = clock();
		duration = (double)(finish - start);
		if (sum != 0){
			cout << "The minimum number of color is " << colorNum << endl;
			cout << "The sum is " << sum << ".And the time is " << duration << endl;
			cout << "The nodes travelled:" << nodeNum << endl;
			break;
		}
		else{
			cout << colorNum << ':' << "No Ans." << endl;
			cout << "The time is " << duration << endl;
		}
	}

	system("PAUSE");
	return 0;
}

void backtrack(int t,int num){ //�����нڵ���л���Ⱦɫ
	nodeNum++;
	if (t > num){ //���t>num��˵�����еĵ㶼�Ѿ���ȷȾɫ����ʱ������
		sum++;
		if (isFirst){
			for (int j = 1; j <= NUM1; j++){
				cout << "The color of node " << j << "is " << ans[j] << endl;
			}
			isFirst = false;
		}
	}
	else{ //��ʱ���е�δ����ȷ��ɫ
		for (int i = 1; i <= colorNum; i++){
			ans[t] = i; //����t����Ⱦ�ɵ�iɫ
			if (ok(t)) //�������ǰt�����ͻ��˵�����У�����Ⱦ��һ����
				backtrack(t + 1, num);
			ans[t] = 0;//����������ɫ�ÿ��Ի���
		}
	}
}

bool ok(int k){ //�жϵ�k���ڵ����ɫ�Ƿ���ǰk-1���ڵ��ͻ
	for (int j = 1; j < k; j++)
		if (arc[k][j] && (ans[j] == ans[k]))
			return false;
	return true;
}

//�������ļ�����
int readData(Book* book, int arc[][50], wchar_t loadFileName[], int sheetNum, int n){ //�����ݴ�excel�ļ��ж���
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
				arc[i+1][j+1] = (int)sheet->readNum(i + 2, j + 2);
				if (arc[i+1][j+1] == -1)
					arc[i+1][j+1] = 0;
				else
					arc[i+1][j+1] = 1;
			}
	}
	book->release();
	return 1;
}
