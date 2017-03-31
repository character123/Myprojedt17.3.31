#include <iostream>
#include <cstdio>
#include <cmath> 
#include "libxl.h" //���ڶ�ȡexcel�ļ�

using namespace libxl; //���ڶ�ȡexcel�ļ�
using namespace std;

#define NUM1 22 
#define NUM2 42

struct closeEdgeNode{
	int adjvex;
	double lowcost;
};

//���ݺ��ļ�����
int readData(Book* book, int id[], double arc[][50], wchar_t loadFileName[], int sheetNum, int n);
//����ķ�㷨����С������
void MiniSpanTree_PRIM(double arc[][50], int id[], int num);

int main(void){
	//��excel�ļ��ж�ȡ���� 
	Book* book = xlCreateBook();
	if (!book){
		cout << "Error when init book." << endl;
		return -1;
	}
	int id[50];
	double arc[50][50];
	wchar_t loadFileName1[] = L"����1-1.��վͼ���ڽӾ���-v1.xls";
	if (readData(book, id, arc, loadFileName1, 1, NUM1) <= 0)
		return -2;
	//����ķ�㷨��22����վ����С������
	MiniSpanTree_PRIM(arc, id, NUM1);

	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	if (readData(book2, id, arc, loadFileName1, 0, NUM2) <= 0)
		return -2;
	//����ķ�㷨��42����վ����С������
	MiniSpanTree_PRIM(arc, id, NUM2);
	system("PAUSE");
	return 0;
}

void MiniSpanTree_PRIM(double arc[][50], int id[], int num){
	//����ķ�㷨����С������
	int u;
	double sum = 0;
	printf("Please input the starting vertex of minimum spanning tree:");
	cin >> u;
	struct closeEdgeNode closedge[50];
	//�õ�u����ʼ�������ڵ��closedge
	int k = u - 1;
	for (int j = 0; j < num; j++)
	{
		if (j != k){
			closedge[j].adjvex = k;// id[k];
			closedge[j].lowcost = arc[k][j];
		}
	}
	closedge[k].lowcost = 0;

	//���closedge�е���Сֵ�����˵������С������S�У�Ȼ���øõ㵽�������ֵ����closedge
	for (int i = 1; i < num; i++){
		int b = 1;
		k = 0;
		for (double min = closedge[0].lowcost; b < num; b++){
			if (min == 0 && closedge[b].lowcost != 0){
				min = closedge[b].lowcost;
				k = b;
			}
			else if (closedge[b].lowcost != 0 && closedge[b].lowcost<min){
				min = closedge[b].lowcost;
				k = b;
			}
		}
		printf("(%d,%d)\n", closedge[k].adjvex + 1, k + 1);
		sum += arc[closedge[k].adjvex][k];
		closedge[k].lowcost = 0;
		for (int j = 0; j < num; j++)
			if (arc[k][j] < closedge[j].lowcost)
			{
				closedge[j].adjvex = k;
				closedge[j].lowcost = arc[k][j];
			}

	}
	cout << "The sum is " << sum << endl;
}

//�������ļ�����
int readData(Book* book, int id[], double arc[][50], wchar_t loadFileName[], int sheetNum, int n){ //�����ݴ�excel�ļ��ж���
	if (book->load(loadFileName)){ //��ȡbook
		cout << "�Ѷ�ȡ�ļ���" << endl;
	}
	else{
		cout << "��ȡ�ļ�ʱ����" << endl;
		return -2;
	}
	Sheet* sheet = book->getSheet(sheetNum);//��ȡexcel�ļ��е�sheet
	if (sheet){ //��sheet�е����ݸ��Ƶ��ṹ������
		for (int i = 0; i < n; i++){
			id[i] = (int)sheet->readNum(1, i + 2);
		}
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++){
				arc[i][j] = sheet->readNum(i + 2, j + 2);
				if (arc[i][j] == -1)
					arc[i][j] = 10000;
			}
	}
	book->release();
	return 1;
}
