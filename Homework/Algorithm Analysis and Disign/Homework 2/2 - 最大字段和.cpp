#include <iostream>
#include <cstdlib>

using namespace std;

float maxSum(int n, float a[], int &x, int &y); //��a[]�е�����ֶκ� 

int main(void){
	//������ֶκ� 
	//���ļ���ȡa��b�����ֶ� 
	FILE * fileTwo, *fileThree;
	fileTwo = fopen("2.txt", "r");
	fileThree = fopen("3.txt", "r");
	int m = 350, n = 180, i = 0;
	float a[350], b[180]; 
	for (i = 0; i < m; i++)
		fscanf(fileTwo, "%f", &a[i]);
	for (i = 0; i < n; i++)
		fscanf(fileThree, "%f", &b[i]);
	
	int aX, aY, bX, bY; //aX��aY�ֱ��Ӧa[]�ֶε�����ֶκ͵���ʼλ�ú���ֹλ�� 
	
	//���a��b������ֶκ� 
	float aSum = maxSum(m, a, aX, aY);
	float bSum = maxSum(n, b, bX, bY);
	cout << "The max sum of subsequence of A is " << aSum << " from " << aX << " to " << aY << endl;
	cout << "The max sum of subsequence of B is " << bSum << " from " << bX << " to " << bY << endl;
	system("PAUSE");
	return 0;
}

float maxSum(int n, float a[], int &x, int &y) { //���a[]������ֶκ� 
	float sum = 0; 
	float b = 0;
	for (int i = 0; i < n; i++){
		if (b >= 0) //����x��i���ֶκ�Ϊ��ʱ��˵����һ����a[i-1]>0 
			b += a[i];
		else{ //�����x��i���ֶκ�Ϊ����˵����һ����a[i-1]<0����i��ʼ���¼�x��sum 
			x = i + 1;
			b = a[i];
		}
		if (b > sum){ //���b>sum�Ļ�������sum=b���ҼǴ�ʱ��iΪ����ֶκ͵���ֹλ��y 
			sum = b;
			y = i + 1;
		}
	}
	return sum; //��������ֶκ� 
}
