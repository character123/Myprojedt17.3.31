#include <iostream>
#include <cstdlib>

#define MAX 601

using namespace std;

void knapsack(int v[], int w[], int c, int n);//��̬�滮����0-1�������������ֵ 
void traceback(int w[], int c, int x[], int n); //�ڵõ�����ֵ������·��������Ž� 

int m[MAX][MAX];//m[i][j]������0-1�������������ֵ������m[i][j]ָ��ѡ��ƷΪi,i+1,...,n������������ʣjʱ��0-1�������������ֵ
//��ôm[1][c]��������0-1�������������ֵ 

int main(void){
	//���ļ��������ݶ�ȡ��w1[],w2[],v1[],v2[]�У�����wi[]��������vi[]����������Ӧ�ļ�ֵ 
	FILE * fileFour;
	fileFour = fopen("4.txt", "r");
	int n1 = 50, n2 = 100, i = 0; //n����Ʒ������i����ʱ���� 
	int w1[51], w2[101], v1[51], v2[101], x1[51], x2[101];
	int c1, c2;  //c��������� 
	fscanf(fileFour, "%d", &c1);
	for (i = 1; i <= n1; i++)
		fscanf(fileFour, "%d", &w1[i]);
	for (i = 1; i <= n1; i++)
		fscanf(fileFour, "%d", &v1[i]);
	fscanf(fileFour, "%d", &c2);
	for (i = 1; i <= n2; i++)
		fscanf(fileFour, "%d", &w2[i]);
	for (i = 1; i <= n2; i++)
		fscanf(fileFour, "%d", &v2[i]);

	//�����һ�������Ľ⣨50����Ʒ���������Ϊ300�� 
	knapsack(v1, w1, c1, n1); //��������ֵ 
	traceback(w1, c1, x1, n1); //��������ֵ�������Ž� 
	cout << "The best value of the pack A is " << m[1][c1] << endl;//������ 
	cout << "The items are:" << endl; 
	int weight = 0, value = 0;
	for (int i = 1; i <= n1; i++)
		if (x1[i] == 1){
			cout << "Item " << i << " Weight: " << w1[i] << " Value: " << v1[i] << endl;
			weight += w1[i];
			value += v1[i];
		}
	cout << "The total weight is " << weight << ' ' << value << endl << endl;

	//����ڶ��������Ľ⣨100����Ʒ���������600�� 
	knapsack(v2, w2, c2, n2);
	traceback(w2, c2, x2, n2);
	cout << "The best value of the pack B is " << m[1][c2] << endl;
	cout << "The items are:" << endl;
	weight = 0;
	value = 0;
	for (int i = 1; i <= n2; i++)
		if (x2[i] == 1){
			cout << "Item " << i << " Weight: " << w2[i] << " Value: " << v2[i] << endl;
			weight += w2[i];
			value += v2[i];
		}
	cout << "The total weight is " << weight << ' ' << value << endl;
	system("PAUSE");
	return 0;
}

void knapsack(int v[], int w[], int c, int n){
	//����v[]��w[]������0-1�������������ֵ��v[]�Ǽ�ֵ��w[]��������c�����������n����Ʒ���� 
	n++; 
	int jMax = (w[n] - 1 > c) ? c : (w[n] - 1); //jMax=min(w[n]-1,c)����jmax=��n����Ʒ����-1��������������еĽ�Сֵ 
	
	for (int j = 0; j <= jMax; j++) //��m[n][0],m[n][1],...,m[n][jMax]=0,����ʣ��n����Ʒ��������ʣ0,1,...,jMaxʱ�����ܽ���Ʒn���� 
		m[n][j] = 0;
	for (int j = w[n - 1]; j <= c; j++) //��m[n][w[n-1]],m[n][w[n]],...,m[n][c]=0,����ʣ��n����Ʒ��������ʣw[n-1],w[n],...,cʱ�����Խ���Ʒn���룬������ӱ����ļ�ֵΪv 
		m[n][j] = v[n];

	for (int i = n - 1; i > 1; i--){
		jMax = (w[i] - 1 > c) ? c : (w[i] - 1); //jMax=min(w[i]-1,c) 
		for (int j = 0; j <= jMax; j++) //��m[i][0],m[i][1],...,m[i][jMax]=m[i+1][j],�����ڵ�i����Ʒ��������ʣ0,1,...,jMaxʱ�����ܽ���Ʒn���룬�ӱ����ļ�ֵû�����i����Ʒʱ����ͬ�� 
			m[i][j] = m[i + 1][j];
		for (int j = w[i]; j <= c; j++) //����m[i][w[i]],m[i][w[i]+1],...,m[i][c]��˵��m[i][j]= max( m[i + 1][j] , m[i + 1][j - w[i]] + v[i] ) 
			m[i][j] = (m[i + 1][j] > (m[i + 1][j - w[i]] + v[i])) ? m[i + 1][j] : (m[i + 1][j - w[i]] + v[i]);
	}
	m[1][c] = m[2][c];
	if (c >= w[1]) //���c����w[1]�Ļ��� m[1][c]= max( m[1][c] , m[2][c - w[1]] + v[1] )
		m[1][c] = (m[1][c] > (m[2][c - w[1]] + v[1])) ? m[1][c] : (m[2][c - w[1]] + v[1]);
}

void traceback(int w[], int c, int x[], int n){ 
	//����m[][]��������0-1������������Ž⣬x[i]���Դ洢��Ʒi�Ƿ񱻷��� 
	for (int i = 1; i < n; i++){ 
		if (m[i][c] == m[i + 1][c]) //���m[i][c]=m[i-1][c]��˵�������Ʒû���Ž�ȥ����x[i]=0 
			x[i] = 0;
		else{ //����x[i]=1������m[i+1][ c-w[i] ]�������������Ž� 
			x[i] = 1;
			c -= w[i];
		}
	}
	x[n] = (m[n][c] > 0) ? 1 : 0; //���m[n]c[c]>0˵����n����Ʒ�����룬��ֹcΪ��������������Խ�� 
}


