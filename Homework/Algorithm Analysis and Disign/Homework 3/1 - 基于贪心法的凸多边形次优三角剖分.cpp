#include <iostream>
#include <cmath> 
#include "libxl.h" //���ڶ�ȡexcel�ļ�

using namespace libxl; //���ڶ�ȡexcel�ļ�
using namespace std;

#define NUM1 21 //��һ���ļ���͹21���Σ���21������ 
#define NUM2 29 //�ڶ����ļ���͹29���Σ���29������ 
#define RADIUM 6378137 //�뾶
const double PI = acos(-1.0); //����PI

struct baseData{ //�����վ���ݵĽṹ
	int num; //��� 
	int enodebid; //��վ���
	double longitude, latitude;  //���Ⱥ�γ��
};

//���ݺ��ļ�����
int readData(Book* book, struct baseData data[], wchar_t loadFileName[], int n);
//��͹����ε����������ʷֶ�Ӧ�ĸ��ߵ�Ȩֵ 
double minWeightTriangulation(int n, struct baseData data[], int &vex);
double dist(struct baseData a, struct baseData b); // dist���ڼ���Va��Vb�ľ��� 
double w(struct baseData * data, int a, int b, int c); //w���ڼ���Va,Vb��Vc���ɵ������ε�Ȩֵ
//ͨ��s[][]���������������ʷ��е��������� 
void Traceback(int n, struct baseData data[], double &sum);

int main(void){
	//��excel�ļ��ж�ȡ���� 
	Book* book = xlCreateBook();
	if (!book){
		cout << "Error when init book." << endl;
		return -1;
	}
	struct baseData data1[30];
	wchar_t loadFileName1[] = L"����3-1.21����վ͹���������.xls";
	if (readData(book, data1, loadFileName1, NUM1) <= 0)
		return -2;
	double sum = 0;
	cout << "21 ����վ͹��������������ʷֵ���������������:" << endl;
	Traceback(20, data1, sum);//�����������ǻ��ֲ�������������

	//��ȡ�ڶ������ݡ�29����վ͹��������ݡ�Ȼ�����29����վ��͹��������������ʷ�
	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	struct baseData data2[30];
	wchar_t loadFileName2[] = L"����3-2.29����վ͹���������.xls";
	if (readData(book2, data2, loadFileName2, NUM2) <= 0)
		return -2;
	cout << endl << "29 ����վ͹��������������ʷֵ���������������:" <<endl;
	Traceback(28, data2, sum);//�����������ǻ��ֲ���������

	system("PAUSE");
	return 0;
}


double minWeightTriangulation(int n, struct baseData data[], int &vex){ //����߳���С������������ɵ������εı߳����������ε�һ�����㷵��
	double min;
	min = w(data, 0, 1, 2); //�Ƚ�V0,V1,V2��ɵ���������ɵ���������Ϊ��Сֵ
	vex = 0;
	for (int i = 1; i <= n - 4; i++){
		if (min > w(data, i, i + 1, i + 2))
		{
			min = w(data, i, i + 1, i + 2);
			vex = i;
		}
	}
	//�ڱ�����V0V1V2,V1V2V3,...,Vn-3Vn-2Vn-1�Ժ�
	if (min > w(data, n - 2, n - 1, 0)){ //��Vn-2Vn-1V0�Ƿ�����Сֵ
		min = w(data, n - 2, n - 1, 0);
		vex = n - 2;
	}
	if (min > w(data, n - 1, 0, 1)){ //��Vn-1V0V1�Ƿ�����Сֵ
		min = w(data, n - 1, 0, 1);
		vex = n - 1;
	}
	return min;
}

void Traceback(int n, struct baseData data[], double &sum){
	sum = 0;
	int i = 0;
	for (; n > 3; n--){
		sum += minWeightTriangulation(n, data, i); //�������С�������α߳��Լ���Ӧ�����
		if (i == n - 2) //����ǵ�n-2������ֱ�����
			cout << data[n - 2].num << "\t" << data[n - 1].num <<"\t" << data[0].num << endl;
		else if (i == n - 1){ //����ǵ�n-1�������������data[0]ɾ��
			cout << data[n - 1].num << "\t" << data[0].num << "\t"<< data[1].num << endl;
			for (int j = 0; j < n - 1; j++)
				data[j] = data[j + 1];
		}
		else{//����������������data[i+1]ɾ��
			cout << data[i].num << "\t" << data[i + 1].num << "\t"<< data[i + 2].num << endl;
			for (int j = i + 1; j < n - 1; j++)
				data[j] = data[j + 1];
		}
	}
	sum += w(data, 0, 1, 2); //������V0V1V2�ӽ�ȥ
	cout << data[0].num << "\t" << data[1].num << "\t" << data[2].num <<endl; //���V0V1V2
	cout << "���������ʷ�ֵΪ��" << sum << endl << endl; //������
}

double w(struct baseData * data, int a, int b, int c){ //����������VaVbVc������֮��
		return (dist(data[a], data[b]) + dist(data[a], data[c]) + dist(data[b], data[c]));
}

double dist(struct baseData a, struct baseData b){ //��֪���Ⱥ�γ�������
	return RADIUM*acos(cos(a.latitude*PI / 180)*cos(b.latitude*PI /180)*cos(a.longitude*PI / 180 - b.longitude*PI / 180) + sin(a.latitude*PI /180)*sin(b.latitude*PI / 180));
}

//�������ļ�����
int readData(Book* book, struct baseData data[], wchar_t loadFileName[], int

	n){ //�����ݴ�excel�ļ��ж���
	if (book->load(loadFileName)){ //��ȡbook
		cout << "�Ѷ�ȡ�ļ���" << endl;
	}
	else{
		cout << "��ȡ�ļ�ʱ����" << endl;
		return -2;
	}
	Sheet* sheet = book->getSheet(1);//��ȡexcel�ļ��е�sheet2 
	if (sheet){ //��sheet�е����ݸ��Ƶ��ṹ������
		for (int i = 0; i < n; i++){
			data[i].num = i + 1;
			data[i].enodebid = (int)sheet->readNum(i + 1, 0);
			data[i].longitude = sheet->readNum(i + 1, 1);
			data[i].latitude = sheet->readNum(i + 1, 2);
		}
	}
	book->release();
	return 1;
}
