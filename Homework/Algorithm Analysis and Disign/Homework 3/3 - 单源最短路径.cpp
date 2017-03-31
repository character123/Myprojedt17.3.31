#include <iostream>
#include <cstdbool>
#include <cstdio>
#include <cmath> 
#include "libxl.h" //���ڶ�ȡexcel�ļ�

using namespace libxl; //���ڶ�ȡexcel�ļ�
using namespace std;

#define NUM1 22 //��һ���ļ���22����վ���ڽӾ���
#define NUM2 42 //�ڶ����ļ���42����վ���ڽӾ���

//���ݺ��ļ�����
int readData(Book* book, int id[], double arc[][50], wchar_t loadFileName[], int sheetNum, int n);
//�Ͻ�˹�����㷨
void shortestPath_DIJ(double arc[][50], int v0, bool p[][50], double d[], int num, int prev[]);
//������·��
void printOut(int prev[], int start, int end);

int main(void){
	//��excel�ļ��ж�ȡ���� 
	Book* book = xlCreateBook();
	if (!book){
		cout << "Error when init book." << endl;
		return -1;
	}

	int id[50];
	double arc[50][50];

	//��ȡ����
	wchar_t loadFileName1[] = L"����1-1.��վͼ���ڽӾ���-v1.xls";
	if (readData(book, id, arc, loadFileName1, 1, NUM1) <= 0)
		return -2;

	bool p[50][50];
	double d[50];
	int prev[50];
	//�Ͻ�˹�����㷨��22����վ��һ�㵽������������·��
	shortestPath_DIJ(arc, 19, p, d, NUM1, prev);
	//�����20���㣨567443������1���㣨33109�������·��
	printOut(prev, 19, 0);

	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	if (readData(book2, id, arc, loadFileName1, 0, NUM2) <= 0)
		return -2;
	//�Ͻ�˹�����㷨��42����վ��һ�㵽������������·��
	shortestPath_DIJ(arc, 15, p, d, NUM2, prev);
	//�����16���㣨565845������2���㣨565667�������·��
	printOut(prev, 15, 2);

	system("PAUSE");
	return 0;
}

void shortestPath_DIJ(double arc[][50], int v0, bool p[][50], double d[], int num, int prev[]) {
	//�Ͻ�˹������V0�������������·����arc�Ǳ߳��ڽӾ���V0��Դ�㣬p[v][w]Ϊtrue��ָV0-V��·������w�㣬d[]�����·���ĳ���
	//num�ǵ��������prev[]�����·����;���㣬ͨ��prev���԰��������·��

	//������V0��ʼ����������
	bool final[50];
	for (int i = 0; i < num; i++){
		final[i] = false;
		d[i] = arc[v0][i];
		for (int j = 0; j < num; j++)
			p[i][j] = false;
		if (d[i] < 10000){
			p[i][v0] = true;
			p[i][i] = true;
		}
		if (d[i] == 10000)
			prev[i] = 0;
		else
			prev[i] = v0;
	}
	d[v0] = 0;
	final[v0] = true;

	//Ȼ�����V0��������·����Сֵ�ĵ�V����V0-V�����¸�������·�����������¼����
	for (int i = 1; i < num; i++){
		double min = 10000;
		int v = 0;
		for (int w = 0; w < num; w++)
			if (!final[w])
				if (d[w] < min){
					v = w;
					min = d[w];
				}
		final[v] = true;
		for (int w = 0; w < num; w++)
			if (!final[w] && (min + arc[v][w] < d[w])){
				//���min+arc[v][w]<d[w]��˵��V0-V-WΪ�µĽ϶�·�������¼�¼����prev[w]��¼Ϊv��
				d[w] = min + arc[v][w];
				for (int i = 0; i < num; i++)
					p[w][i] = p[v][i];
				p[w][w] = true;
				prev[w] = v;
			}
	}

	//������
	for (int i = 0; i < num; i++){
		cout << "Port " << i << endl;
		for (int j = 0; j < num; j++)
			if (p[i][j] == true)
				cout << j << " ";
		cout << endl << d[i] << endl;
	}
	cout << endl;
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
	Sheet* sheet = book->getSheet(sheetNum);//��ȡexcel�ļ��е�sheet2 
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

void printOut(int prev[], int start, int end){ //�ݹ鵹�Ƴ����·������end��start
	cout << end << ' ';
	if (start != end){
		printOut(prev, start, prev[end]);
	}
}