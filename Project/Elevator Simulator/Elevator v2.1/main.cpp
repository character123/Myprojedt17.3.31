#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "graphics.h"
#include "types.h"
#include "prototypes.h"

HANDLE hMutex; //���廥�����

int main(void)
{
	//���б����Ķ���ͳ�ʼ��
	int timeCount = 0;  //��ʱ����ÿѭ��һ��ģ��1s
	struct REQ reqs[20];  //�ݴ�ÿ�λ�õ�ĳʱ�̵�ȫ�������¼�
	struct REQ reqTemp; //�����Ļ��������õ������ݴ棬��������ɾ���������Ƶ���Ļ����У�ʹ��ȫ�����󶼿�����ʾ
	int reqCount, reqTime;// //�ݴ�ÿ�λ�õ�ĳʱ�̵������¼������Լ������ʱ��
	int inputMethod = 2, serviceMethod = 1; //����ģʽ�Ͳ���ģʽ����ʼΪ��������������ȷ������
	enum STATE  preState;//�洢����ǰһ״̬ 
	LISTNODEPTR headp;//�������������ͷָ��
	FILE *fpout, *fpout2;  //ָ�������ļ�������ļ�File�ṹ
	struct ELEVATOR elevator;//�洢���ݶ�����Ϣ����������״̬������¥�㡢ͣ��ʱ��ȵ�
	reqs[0].reqAct = EMPTY;//����һ�������ʼ��ΪEMPTY�Է�ֹ��IDLE�л�����ʱ����addServList������ָ�ֵ
	reqs[0].reqF = 1;

	//������������ͷ�ڵ㡣��ͷ�ڵ㲻ʵ�ʴ洢����
	headp = (LISTNODEPTR)malloc(sizeof(LISTNODE));
	(headp->reqData).reqAct = EMPTY;//��ͷ�������ݽ��г�ʼ�� 
	(headp->reqData).reqF = 0;
	headp->nextPtr = NULL;

	//��ʼ�����̼��ַ���ݵĽṹ����
	struct HANDLETRANS handleTrans;{
		handleTrans.headp = headp;//��������ͷ���
		handleTrans.elevatorPtr = &elevator;//����״̬�ĵ�ַ
		handleTrans.serviceMethodPtr = &serviceMethod;//����ģʽ
		handleTrans.reqCountPtr = &reqCount;//������
		handleTrans.reqTimePtr = &reqTime;//��������ʱ��
		handleTrans.reqTempPtr = &reqTemp;//��ʱ���������
	}

	//��ʼ���������沢���ñ���ɫ
	initgraph(768, 672);
	setbkcolor(WHITE);

	//������������Ľ���
	HANDLE hThread1; //������� 
	hMutex=CreateMutex(NULL,FALSE,NULL); //����������� 
	hThread1=CreateThread(NULL,0,keyboardInputProc,&handleTrans,0,NULL); //��������������̣������������� 
	CloseHandle(hThread1); //�رվ��

	//������ļ�
	//�����ļ���������ļ�output.txt��������״̬�б仯ʱ�ͻ����ļ������   
	if (!openFileToWrite("output.txt", &fpout)){
		printf("Output file \"output.txt \" can't be opened.\n");
		return 0;
	}
	//�����ļ���������ļ�outputDetail.txt���������ļ������������Ϣ
	if (!openFileToWrite("outputDetail.txt", &fpout2)){//�����ļ�������������ļ�   
		printf("Output file \"outputDetail.txt \" can't be opened.\n");
		return 0;
	}

	//��ʼ�����ݽṹ��Ϣ�����ݳ�ʼλ��1�㣬Ϊ����״̬�� 
	initElevator(&elevator);

	//��ʼģ���������
	//��������

	//���� ͼ����� �Ľ���
	HANDLE hThread3; //������� 
	hThread3 = CreateThread(NULL, 0, graphicOutProc, &handleTrans, 0, NULL); //����ͼ��������̣������������� 
	CloseHandle(hThread3);//�رվ��

	//���� ����������� �Ľ��� ��Ϊ˳�����
	HANDLE hThread2; //������� 
	hThread2=CreateThread(NULL,0,addServListProc,&handleTrans,0,NULL); //��������������̣������������� 
	CloseHandle(hThread2);//�رվ��

	//���� ������� �Ľ���
	HANDLE hThread4; //������� 
	hThread4 = CreateThread(NULL, 0, mouseInputProc, &handleTrans, 0, NULL); //�������������̣������������� 
	CloseHandle(hThread4);

	//�����������ѭ�������ϼ������״̬��Ȼ�󽫵���״̬������ļ���
	while (1){
		//1.�������������ڼ��̽����� 
		preState = elevator.eleState;//��¼ǰһ״̬
		WaitForSingleObject(hMutex, INFINITE); //�ȴ����������Ӧ 
		runService(&elevator, headp, &reqTemp);//���ݵ��ݵ�ǰ״̬������һ״̬
		ReleaseMutex(hMutex);//�ͷŻ������ 
		//2.�������״̬
		//�������״̬��Ϣ���ļ�2��
		WaitForSingleObject(hMutex, INFINITE);//���뻥�����
		set_fileOutput(fpout2, timeCount, elevator, headp, 1);//������ݼ�¼��ָ��fpout2ָ����ļ���
		if (elevator.eleState != preState)//��״̬�仯������ǰʱ�䡢״̬�͵ȴ����е����д�뵽�ļ���
			set_fileOutput(fpout, timeCount, elevator, headp, 2);
		ReleaseMutex(hMutex);//�ͷŻ������
		//3.ʱ��Ƭ��1
		timeCount++;
		Sleep(1000);//�ȴ�1000ms
	}//End of while

	//�ر�����ļ�
	fclose(fpout);
	fclose(fpout2);

	system("pause");
	return 0;
}//end main
