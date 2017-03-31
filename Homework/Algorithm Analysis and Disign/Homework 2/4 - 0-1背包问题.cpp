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
	double longitude,latitude;  //���Ⱥ�γ��
};

//���ݺ��ļ�����
int readData(Book* book, struct baseData data[], wchar_t loadFileName[], int n);
//��͹����ε����������ʷֶ�Ӧ�ĸ��ߵ�Ȩֵ 
void minWeightTriangulation(int n, double t[][30], int s[][30], struct baseData * data);
double dist(struct baseData a, struct baseData b); // dist���ڼ���Va��Vb�ľ��� 
double w(struct baseData * data, int a, int b, int c); //w���ڼ���Va,Vb��Vc���ɵ������ε�Ȩֵ 
//ͨ��s[][]���������������ʷ��е��������� 
void Traceback(int i, int j, int s[][30]);

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
		
	//����t[][]��s[][]����ʼ����t[i][j]��¼�˴�Vi-1��Vk����Vj��ɵ�͹����ε����������ʷֵĸ��ߵ�Ȩֵ
	//��s[i][j]���¼����Vi-1��Vjһ�𹹳������εĶ��� 
	double t[30][30];
	int s[30][30];
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 30; j++){
			t[i][j] = 0;
			s[i][j] = 0;
		}
	//����͹����ε����������ʷ�����Ӧ��Ȩ����ֵ 
	minWeightTriangulation(NUM1 - 1, t, s, data1);
	cout << endl << t[1][NUM1-1] << endl;
	//����s[][]���������������ʷֵĽ� 
	Traceback(1, NUM1 - 1, s);
	
	//��ȡ�ڶ������ݡ�29����վ͹��������ݡ������³�ʼ��s[][]��t[][] 
	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	struct baseData data2[30];
	wchar_t loadFileName2[] = L"����3-2.29����վ͹���������.xls";
	if (readData(book2, data2, loadFileName2, NUM2) <= 0)
		return -2;
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 30; j++){
			s[i][j] = 0;
			t[i][j] = 0;
		}
		
	
	minWeightTriangulation(NUM2 - 1, t, s, data2);
	cout << endl << t[1][NUM2 - 1] << endl;
	Traceback(1, NUM2 - 1, s);
	system("PAUSE");
	return 0;
}

void minWeightTriangulation(int n, double t[][30], int s[][30],struct baseData * data){
	//��͹����ε����������ʷֶ�Ӧ�ĸ��ߵ�Ȩֵ
	for (int i = 0; i <= n; i++) //��i=jʱ��t[i][j]=0
		t[i][i] = 0;
	for (int r = 2; r <= n; r++)
		for (int i = 1; i <= n - r + 1; i++){
			int j = i + r - 1;
			t[i][j] = t[i + 1][j] + w(data, i - 1, i, j); //��i<jʱt[i][j]=t[i+1][j]+w(i-1,i,j)����͹�����Vi...Vj=(͹�����Vi+1...Vj)+������Vi-1ViVj 
			s[i][j] = i;
			for (int k = i + 1; k < i + r - 1; k++){ //k��i������j��������t[i][k]<t[i][j]ʱ ����t[i][j]��s[i][j] 
				double u = t[i][k] + t[k + 1][j] + w(data, i - 1, k, j);
				if (u < t[i][j]){
					t[i][j] = u;
					s[i][j] = k;
				}
			}
		}
}

double w(struct baseData * data, int a, int b, int c){ //����������VaVbVc������֮�� 
	return (dist(data[a], data[b]) + dist(data[a], data[c]) + dist(data[b], data[c]));
}

double dist(struct baseData a, struct baseData b){ //��֪���Ⱥ�γ�������
	return RADIUM*acos(cos(a.latitude*PI / 180)*cos(b.latitude*PI / 180)*cos(a.longitude*PI / 180 - b.longitude*PI / 180) + sin(a.latitude*PI / 180)*sin(b.latitude*PI / 180));
}

void Traceback(int i, int j, int s[][30]){ //����s[][]���Ƶ����������ʷֵĽ� 
	if (i == j) return;
	cout << i - 1 << "," << s[i][j] << "," << j << endl;
	Traceback(i, s[i][j], s);
	Traceback(s[i][j] + 1, j, s);
}

//�������ļ�����
int readData(Book* book, struct baseData data[], wchar_t loadFileName[], int n){ //�����ݴ�excel�ļ��ж���
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
			data[i].enodebid = (int)sheet->readNum(i+1, 0);
			data[i].longitude = sheet->readNum(i+1, 1);
			data[i].latitude = sheet->readNum(i+1, 2);
		}
	}
	book->release();
	return 1;
}
