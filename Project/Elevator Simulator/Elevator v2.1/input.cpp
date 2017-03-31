#include <stdio.h> 
#include <windows.h>
#include "graphics.h"
#include "types.h"
#include "prototypes.h"

extern HANDLE hMutex; //���廥����� 

//input.c

DWORD WINAPI keyboardInputProc(LPVOID lpParameter){ 
	//����ģʽ���������̶�ȡ���� 
	struct REQ reqs[10];//��ΪaddServList�����ĸ�ʽ�����reqs����Ϊ���飬ͬʱ��reqCountʼ�ն���Ϊ1 
	reqs[0].reqAct = EMPTY;//��ʼ��
	reqs[0].reqF = 1;
	int reqCount=1;
	struct HANDLETRANS * handleTransPtr = (struct HANDLETRANS *) lpParameter;//������������ʽ�� 
		LISTNODEPTR headp = (* handleTransPtr).headp;//��ȡ����ͷ���ĵ�ַ
		struct ELEVATOR * elevatorPtr = (* handleTransPtr).elevatorPtr;//��ȡ����״̬�ĵ�ַ
		int * serviceMethodPtr =  (* handleTransPtr).serviceMethodPtr;//��ȡ����ģʽ�ĵ�ַ
		struct REQ * reqTempPtr =(* handleTransPtr).reqTempPtr;//��ȡ��ʱ����ṹ�ĵ�ַ

	while (1){ //����ѭ�� 
		keyboardInput(reqs,serviceMethodPtr); //���ü������뺯���ȴ���ȡ�������� 
		if (reqs[0].reqAct != EMPTY){
			WaitForSingleObject(hMutex, INFINITE);//���û��м�������ʱ�����뻥����� 
			addServList((*serviceMethodPtr), headp, reqs, reqCount, elevatorPtr, reqTempPtr);//����ȡ��������������� 
			ReleaseMutex(hMutex);//�ͷŻ������
		}
	}
}

/*����ȡ���ַ��������ת����������������*/
void keyboardInput(struct REQ reqs[], int * serviceMethodPtr){ 
	char ch;
	ch = getch(); //��ȡ�ַ� 
	if(ch <= '9' && ch >= '1'){ //���ַ���1-9ʱ��ʶ������Ϊ�ڲ����� 
		reqs[0].reqAct = INSIDE;
		reqs[0].reqF = (int)ch-48;
	}
	else if (ch == 'z' || ch == 'x'){ //���ַ���z��xʱ��ʶ��Ϊ���Ź������� 
		if(ch == 'z'){
			reqs[0].reqAct = OPEN;
			reqs[0].reqF = 0;
		}
		else {
			reqs[0].reqAct = CLOSE;
			reqs[0].reqF = 0;
		}
	}
	else {
		switch (ch){ //�������ַ�����ʶ�� 
			case ('q')://8 qwertyui��Ӧ�����ⲿ���� 
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 8;
				break;
			case ('w'):case ('t')://7&4
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = (int)ch-112;
				break;
			case ('e')://6
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 6;
				break;
			case ('r')://5
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 5;
				break;
			case ('y')://3
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 3;
				break;
			case ('u')://2
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 2;
				break;
			case ('i')://1
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 1;
				break;
			case ('a')://9 asdfghj��Ӧ�����ⲿ���� 
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 9;
				break;
			case ('s')://8
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 8;
				break;
			case ('d')://7
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 7;
				break;
			case ('f')://6
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 6;
				break;
			case ('g')://5
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 5;
				break;
			case ('h')://4
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 4;
				break;
			case ('j')://3
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 3;
				break;
			case ('k')://2
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 2;
				break;
			case ('c')://����Ϊ�����ȷ������
				(*serviceMethodPtr) = 1;
				break;
			case ('v')://����Ϊ˳��������
				(*serviceMethodPtr) = 2;
				break;
			case ('b') ://����Ϊ�긣���ݲ���
				(*serviceMethodPtr) = 3;
				break;
			case ('m') ://�˳�
				exit(0);
		}
	}
}

/*���ģʽ����������ȡ����*/
DWORD WINAPI mouseInputProc(LPVOID lpParameter){
	struct REQ reqs[10]; //��ΪaddServList�����ĸ�ʽ�����reqs����Ϊ���飬ͬʱ��reqCountʼ�ն���Ϊ1
	reqs[0].reqAct = EMPTY;//Ϊ�˷�ֹ�л�����ʱ���������������ĵ�һ����ʼ��
	reqs[0].reqF = 1;
	int reqCount = 1;//�̶�ÿһ�λ�õ�����Ϊ1��
	struct HANDLETRANS * handleTransPtr = (struct HANDLETRANS *) lpParameter; //������������ʽ�� 
		LISTNODEPTR headp = (*handleTransPtr).headp;//��ȡ����ͷ���ĵ�ַ
		struct ELEVATOR * elevatorPtr = (*handleTransPtr).elevatorPtr;//��ȡ����״̬�ĵ�ַ
		int * serviceMethodPtr = (*handleTransPtr).serviceMethodPtr;//��ȡ����ģʽ�ĵ�ַ
		struct REQ * reqTempPtr = (*handleTransPtr).reqTempPtr;//��ȡ��ʱ�洢����ĵ�ַ
	while (1){ //����ѭ�� 
		for (; is_run(); delay_fps(60)){
			mouseInput(reqs, serviceMethodPtr);//���ü������뺯���ȴ���ȡ�������� 
			if (reqs[0].reqAct != EMPTY){//��ֹ���л�����ʱû���������������������
				WaitForSingleObject(hMutex, INFINITE); //���û��м�������ʱ�����뻥����� 
				addServList((*serviceMethodPtr), headp, reqs, reqCount, elevatorPtr, reqTempPtr); //����ȡ���������������
				ReleaseMutex(hMutex);//�ͷŻ������
			}//if
		}//for
	}//while
}

void mouseInput(struct REQ reqs[], int * serviceMethodPtr){
	//��ȡ�����Ϣ�����������ȴ����ȴ�������ϢΪֹ
	mouse_msg msg = { 0 };//��ʼ�������Ϣ����
	msg = getmouse();//��һ�λ�������Ϣ
	while ((int)msg.is_down() ==0)//ֱ����걻����ʱ������ѭ��
	{
		msg = getmouse();
	}
	if ((int)msg.is_down() == 1){//����걻����ʱ
		if (msg.x >= 576 && msg.x < 624){//�ڲ�����6-9¥�����ż�
			if (msg.y >= 240 && msg.y < 432){//�ڲ�����6-9¥��
				reqs[0].reqAct = INSIDE;
				reqs[0].reqF = 9 - (((int)msg.y - 240) / 48);
			}
			else if (msg.y >= 192 && msg.y < 240){//���ż�
				reqs[0].reqAct = OPEN;
				reqs[0].reqF = 0;
			}
		}
		else if (msg.x >= 624 && msg.x < 672){//�ڲ�����1-5¥������ż�
			if (msg.y >= 336 && msg.y < 576){//�ڲ�����1-5¥��
				reqs[0].reqAct = INSIDE;
				reqs[0].reqF = 5 - (((int)msg.y - 336) / 48);
			}
			else if (msg.y >= 192 && msg.y < 240){//���ż�
				reqs[0].reqAct = CLOSE;
				reqs[0].reqF = 0;
			}
		}
		else if (msg.x>192 && msg.x < 240){//�ⲿ����
			if (msg.y >= 144 && msg.y < 528){
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 8 - (((int)msg.y - 144) / 48);
			}
		}
		else if (msg.x>240 && msg.x < 288){//�ⲿ����
			if (msg.y >= 96 && msg.y < 480){
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 9 - (((int)msg.y - 96) / 48);
			}
		}
		else if (msg.x>336 && msg.x < 528){//�л��������ȷ���
			if (msg.y >= 288 && msg.y <= 336){
				(*serviceMethodPtr) = 1;
			}
			else if (msg.y > 336 && msg.y <= 384){//�л���˳�����
				(*serviceMethodPtr) = 2;
			}
			else if (msg.y > 384 && msg.y <= 432){
				(*serviceMethodPtr) = 3;
			}
		}
	}
}

//end of input.c