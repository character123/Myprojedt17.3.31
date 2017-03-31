#include <stdio.h>
#include <windows.h>
#include "types.h"
#include "prototypes.h"

//output.c

int openFileToWrite(char inFileName[], FILE * * fpout){
	//�����ļ�������������ļ� ,�ļ��ṹ������fpoutָ�򡣴򿪳ɹ�������1�����򷵻�0
	if (((*fpout) = fopen(inFileName, "w+")) == NULL)
		return 0;
	else
		return 1;
}

void set_fileOutput(FILE *fpout, int time_count, struct ELEVATOR elevator, LISTNODEPTR headp, int detail){
	//������״̬�����output.txt��outputDetail.txt 
	//fpout��ָ��Ҫ��д����ļ��Ľṹ������timeCount���˿̵�ʱ��Ƭ
	//detail:Ϊ1����д�� outputDetail.txt�У�Ϊ0����д�� output.txt��
	char * str1[6] = { "IDLE", "UP", "DOWN", "DOOROPENING", "DOORCLOSING", "STOP" };//�洢״̬�����ַ����ĵ�ַ
	char * str2[5] = { "T", "U", "D", "o", "c" };//�洢ö�ٳ����ַ����ĵ�ַ
	LISTNODEPTR currentPtr = headp;//���������ͷ���

	switch (detail){
	case (1) :
		//������ϸ��¼
		fprintf(fpout, "T=%3d,State=%11s,NowF=%d+%d,GoalF=%d,StopT=%d,WaitF=",
		time_count, str1[elevator.eleState], elevator.nowF, elevator.stateTime, elevator.goalF, elevator.stopTime);
		while (currentPtr->nextPtr != NULL){ //������Ϊ��ʱ�����Ŀ��¥������Ӧ������
			fprintf(fpout, "%d%s ", (currentPtr->nextPtr)->reqData.reqF,
				str2[(currentPtr->nextPtr)->reqData.reqAct]);
			currentPtr = currentPtr->nextPtr;//�Ƶ���һ�ڵ㴦
		}
		fprintf(fpout, "\n");//��ǰʱ�������¼��ϣ����л��д���
		break;
	case (2) :
		//����״̬�仯��¼
		fprintf(fpout, "T=%3d,State=%11s,NowF=%d,GoalF=%d,StopT=%d,WaitF=",
		time_count, str1[elevator.eleState], elevator.nowF, elevator.goalF, elevator.stopTime);
		while (currentPtr->nextPtr != NULL){ //������Ϊ��ʱ����� Ŀ��¥������Ӧ������
			fprintf(fpout, "%d%s ", (currentPtr->nextPtr)->reqData.reqF,
				str2[(currentPtr->nextPtr)->reqData.reqAct]);
			currentPtr = currentPtr->nextPtr;//�Ƶ���һ�ڵ㴦
		}
		fprintf(fpout, "\n");//��ǰʱ�������¼��ϣ����л��д���
		break;
	}
}

//end of output.c