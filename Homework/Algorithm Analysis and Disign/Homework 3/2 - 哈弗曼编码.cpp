#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdbool>

using namespace std;

struct HuffmanNode * minValue(struct HuffmanNode * freqTree);
void printTree(struct HuffmanNode * treeNode, int layer, char a[]);

struct HuffmanNode{ //�ڱ��Ź�������ʱ���õĽڵ�
	char text; //�ַ�����
	int weight; //����Ƶ��
	bool leaf; //�ýڵ��ڹ����������Ƿ���Ҷ�ӽڵ�
	struct HuffmanNode * lchild; //����
	struct HuffmanNode * rchild; //�Һ���
	struct HuffmanNode * next; //��������ָ����һ���ڵ�
};

struct HuffmanCode{ //27���ַ��Ĺ���������
	char code[20]; //����
	int length; //���볤��
};

int value = 0; //�����ܳ���
struct HuffmanCode code[27]; //27���ַ��Ĺ��������루����26��Сд��ĸ��'#'�ţ�

int main(void){
	//���Ƚ�����Դ���ֶ���
	FILE * fileOne;
	fileOne = fopen("1.txt", "r");
	char text[1020];
	fgets(text, 1020, fileOne);
	printf("%s", text);

	//Ȼ����һ�������������������ʼ��
	struct HuffmanNode * freqTree = new HuffmanNode;
	freqTree->weight = INFINITY;
	freqTree->text = 0;
	freqTree->next = NULL;

	struct HuffmanNode * lastNode = freqTree;
	
	//�����������е��ַ������������У�ÿһ���ַ���Ӧһ���ڵ�
	for (int i = 0; text[i] != '\n'; i++){
		while (lastNode->next != NULL){
			if (lastNode->next->text == text[i])
				break;
			lastNode = lastNode->next;
		}
		if (lastNode->next == NULL){
			lastNode->next = new HuffmanNode;
			lastNode = lastNode->next;
			lastNode->leaf = true;
			lastNode->text = text[i];
			lastNode->weight = 1;
			lastNode->lchild = NULL;
			lastNode->rchild = NULL;
			lastNode->next = NULL;
		}
		else {
			lastNode->next->weight++;
		}
		lastNode = freqTree;
	}
	while (lastNode->next != NULL){
		cout << lastNode->next->text << ' ' << lastNode->next->weight << endl;;
		lastNode = lastNode->next;
	}

	//��������������Ƶ��������ϳɹ�������
	lastNode = freqTree;
	while (lastNode->next != NULL){
		//���������ҵ���СƵ�ʵĽڵ�
		struct HuffmanNode * minLastNode1 = minValue(freqTree);
		struct HuffmanNode * minValueNode1 = minLastNode1->next;
		minLastNode1->next = minLastNode1->next->next;
		//ȥ����С�Ժ��ҵ���СƵ�ʵĽڵ�
		struct HuffmanNode * minLastNode2 = minValue(freqTree);
		struct HuffmanNode * minValueNode2 = minLastNode2->next;
		minLastNode2->next = minLastNode2->next->next;
		//��һ���½ڵ㣬�����Ͻڵ㲢�����Һ������棬Ȼ������ڵ�Ż�������
		struct HuffmanNode * treeNode = new struct HuffmanNode;
		treeNode->leaf = false;
		treeNode->text = 0;
		if (minValueNode1->weight >= minValueNode2->weight){
			treeNode->lchild = minValueNode1;
			treeNode->rchild = minValueNode2;
		}
		else{
			treeNode->lchild = minValueNode2;
			treeNode->rchild = minValueNode1;
		}
		treeNode->next = NULL;
		treeNode->weight = minValueNode1->weight + minValueNode2->weight;
		if (freqTree->next == NULL){
			freqTree->next = treeNode;
			break;
		}
		else{
			struct HuffmanNode * temp = freqTree;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = treeNode;
		}
	}
	cout << endl << endl;
	char a[20];
	//���������
	printTree(freqTree->next, 0, a);

	//�������Ľ�����
	cout << endl << value << endl;
	cout << endl << endl;
	for (int i = 0; text[i] != '\0'; i++){
		if (text[i] == '#'){
			cout << code[0].code;
		}
		else{
			cout << code[text[i] - 96].code;
		}
	}

	fclose(fileOne);
	system("PAUSE");
	return 0;
}

struct HuffmanNode * minValue(struct HuffmanNode * freqTree){ //���������ҵ���Сֵ
	struct HuffmanNode * lastNode = freqTree;
	struct HuffmanNode * minLastNode = freqTree;
	while (lastNode->next != NULL){
		if (lastNode->next->weight <= minLastNode->next->weight){//!!
			minLastNode = lastNode;
		}
		lastNode = lastNode->next;
	}
	return minLastNode;
}

void printTree(struct HuffmanNode * treeNode, int layer, char a[]){ //�ݹ�Թ��������еĽڵ���б��벢���
	if (treeNode->leaf == true){
		cout << treeNode->text << ' ' << treeNode->weight << endl;
		value += treeNode->weight*layer;
		if (treeNode->text == '#'){
			strcpy(code[0].code, a);
			code[0].length = layer;
			cout << code[0].code << ' ' << code[0].length << endl;
		}
		else{
			strcpy(code[treeNode->text - 96].code, a);
			code[treeNode->text - 96].length = layer;
			cout << code[treeNode->text - 96].code << ' ' << code[treeNode->text - 96].length << endl;
		}
	}
	else{
		if (treeNode->lchild){
			a[layer] = '0';
			a[layer + 1] = '\0';
			printTree(treeNode->lchild, layer + 1, a);
		}
		if (treeNode->rchild){
			a[layer] = '1';
			a[layer + 1] = '\0';
			printTree(treeNode->rchild, layer + 1, a);
		}
	}
}
