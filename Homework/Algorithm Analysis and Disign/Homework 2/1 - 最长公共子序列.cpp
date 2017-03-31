#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

int c[600][600], b[600][600]; //c[i][j]�洢x[i]��y[j]��������еĳ��ȣ���b[i][j]��洢c[i][j]������һ��������Ľ�õ��� 
//b[][]��������ȡֵ��1��2��3�ֱ��ӦZk=Xm=Yn��Xm!=Yn&Zk!=Xm��Xm!=Yn&Zk!=Yn�������  

void lcsLength(char x[], char y[], int c[][600], int b[][600]); //lcsLength������Դ����X��Y������������еĳ��� 
void lcs(int i, int j, char x[], int b[][600]); //ͨ��b[][]���Թ��������������У�b[][]��¼��ԭ����������һ��������Ľ�õ���

int main(void){
	//��������������
	//���ļ��ж�ȡ����a1��a2��a3��a4 
	FILE * fileOne; 
	fileOne = fopen("1.txt", "r");
	char a1[600], a2[600], a3[600], a4[600];
	char trash[600];
	fgets(trash, 600, fileOne);
	fgets(a1, 600, fileOne);
	fgets(trash, 600, fileOne);
	fgets(a2, 600, fileOne);
	fgets(trash, 600, fileOne);
	fgets(a3, 600, fileOne);
	fgets(trash, 600, fileOne);
	fgets(a4, 600, fileOne);
	
	//������a1��a2������������в������� 
	lcsLength(a1, a2, c, b);
	cout << "The longest common substring of A and B is:" << endl;
	lcs(strlen(a1), strlen(a2), a, y);
	
	//������a3��a4������������в������� 
	lcsLength(a3, a4, x, y);
	cout << "The longest common substring of C and D is:" << endl;
	lcs(strlen(a3), strlen(a4, c, y);
	
	//������a1��a4������������в������� 
	lcsLength(a1, a4, x, y);
	cout << "The longest common substring of A and D is:" << endl;
	lcs(strlen(a1), strlen(a4), a, y);
	
	//end 
	fclose(fileOne);
	system("PAUSE");
	return 0;
}

void lcsLength(char x[], char y[], int c[][600], int b[][600]){
	//����x��y�ĳ��� 
	int m = strlen(x);
	int n = strlen(y) - 1;
	//������ʱ���� 
	int i = 0, j = 0;
	
	//��i=0��j=0ʱ��c[i][j]=0
	for (i = 0; i <= m; i++)
		c[i][0] = 0;
	for (i = 0; i <= n; i++)
		c[0][i] = 0;
	
	for (i = 1; i <= m; i++)
		for (j = 1; j <= n; j++){
			//��x[i]=y[j]ʱ��c[i][j] = c[i - 1][j - 1] + 1����Ӧ��һ����� 
			if (x[i-1] == y[j-1]){
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = 1;
			}
			//��x[i]!=y[j]ʱ��c[i][j]=max(c[i-1][j], c[i][j-1])���ֱ��Ӧ�ڶ�������͵�������� 
			else if (c[i - 1][j] >= c[i][j - 1]){
				c[i][j] = c[i - 1][j];
				b[i][j] = 2;
			}
			else{
				c[i][j] = c[i][j - 1];
				b[i][j] = 3;
			}
		}
}

void lcs(int i, int j, char x[], int b[][600]){ //����b[][]������������� 
	if (i == 0 || j == 0)
		return;
	if (b[i][j] == 1){ //��һ����� ��x[i]=y[j]��Zk = Zk-1 + z[k]
		lcs(i - 1, j - 1, x, b);
		cout << x[i-1];
	}
	else if (b[i][j] == 2){ //�ڶ�������� x[i]!=y[j]����X[i-1]��Y[j]�м������� 
		lcs(i - 1, j, x, b);
	}
	else{ //����������� x[i]!=y[j]����X[i]��Y[j-1]�м�������
		lcs(i, j - 1, x, b);
	}
}
