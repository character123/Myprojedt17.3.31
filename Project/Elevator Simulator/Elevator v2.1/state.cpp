#include <stdio.h>
#include <windows.h>
#include "types.h"
#include "prototypes.h"

extern HANDLE hMutex; //���廥����� 

//state.c

/*����������е�״̬*/
//struct REQ * reqTempPtr����ʱ����Ѿ�����Ŀ��¥�������������Ļ�����
void runService(struct ELEVATOR * elevator, LISTNODEPTR headp, struct REQ * reqTempPtr){
	int actionUsed = 0; //������ִ�����ں���βɾ������
	struct REQ actionTemp;//��ʱ����ṹ����������е�һ������
	LISTNODEPTR nowPtr;//����ĵ�ǰλ��

	nowPtr = headp->nextPtr; //��nowPtr��ʼ�� 
	if (nowPtr == NULL) //���û����������ʱ���������ʶΪ�� 
		actionTemp.reqAct = EMPTY;
	else{//������ʱ������ǰ�ڵ�ָ��Ľṹ����������ʱ����ṹ
		actionTemp = nowPtr->reqData;
	}

	switch (elevator->eleState){ //�жϵ���״̬ 
	case (IDLE) : //���� 
		if (actionTemp.reqAct != -1){
			if (actionTemp.reqAct == OPEN || actionTemp.reqAct == CLOSE){//��ǰ����Ϊ����������
				if (actionTemp.reqAct == OPEN){//��������
					elevator->eleState = DOOROPENING;//�޸ĵ���״̬
					elevator->stateTime = OPENLEN;//�޸ĵ�ǰ״̬ʱ��
				}
			}
			else if (actionTemp.reqF > elevator->nowF){ //���� 
				elevator->eleState = UP;//�޸ĵ���״̬
				elevator->goalF = actionTemp.reqF;//�޸ĵ��ݵ�Ŀ��¥��
				elevator->stateTime = ONELAYERLEN;//�޸ĵ�ǰ״̬ʱ��
			}
			else if (actionTemp.reqF < elevator->nowF){ //�½� 
				elevator->eleState = DOWN;//�޸ĵ���״̬
				elevator->goalF = actionTemp.reqF;//�޸ĵ��ݵ�Ŀ��¥��
				elevator->stateTime = ONELAYERLEN;//�޸ĵ�ǰ״̬ʱ��
			}
			else if (actionTemp.reqF == elevator->nowF){ //�������� 
				elevator->eleState = DOOROPENING;//�޸ĵ���״̬
				elevator->stateTime = OPENLEN;//�޸ĵ�ǰ״̬ʱ��
			}
			actionUsed = 1;//��ǰ���ִ�У���������Ϊ1
		}
		(*reqTempPtr).reqAct = EMPTY;//��ʼ��reqTempPtrָ��Ľṹ
		(*reqTempPtr).reqF = 0;
		break;
	case (DOOROPENING) : //���� 
		if (actionTemp.reqAct == CLOSE){ //���¹��ż��������ѿ���ʱ����� 
			elevator->eleState = DOORCLOSING;//�޸ĵ���״̬
			elevator->stateTime = OPENLEN - elevator->stateTime;//�޸ĵ�ǰ״̬ʱ��
			actionUsed = 1;//��ǰ���ִ�У���������Ϊ1
		}
		else if (elevator->stateTime == 0){ //������� 
			elevator->eleState = STOP;//�޸ĵ���״̬
			elevator->stateTime = STOPLEN;//�޸ĵ�ǰ״̬ʱ��
		}
		else{ //����δ��� 
			elevator->stateTime--;//�޸ĵ�ǰ״̬ʱ�䣬״̬ʱ���һ
		}
		break;
	case (STOP) : //ͣ��
		if (actionTemp.reqAct == OPEN){ //���¿��ż����Ⱥ�ʱ������ 
			elevator->stateTime = STOPLEN;//�޸ĵ�ǰ״̬ʱ��
			actionUsed = 1;//��ǰ���ִ�У���������Ϊ1
		}
		else if (actionTemp.reqAct == CLOSE || elevator->stateTime == 0){ //���¹��ż����ߵȺ�ʱ�䵽 
			elevator->eleState = DOORCLOSING;//�޸ĵ���״̬
			elevator->stateTime = CLOSELEN;//�޸ĵ�ǰ״̬ʱ��
			if (actionTemp.reqAct == CLOSE)
				actionUsed = 1;//��ǰ���ִ�У���������Ϊ1
		}
		else//�Ⱥ�ʱ��δ��
			elevator->stateTime--; //�޸ĵ�ǰ״̬ʱ�䣬״̬ʱ���һ
		break;
	case (DOORCLOSING) : //���� 
		if (actionTemp.reqAct == OPEN){ //���¿��ż��������ѹ���ʱ�俪�� 
			elevator->eleState = DOOROPENING;//�޸ĵ���״̬
			elevator->stateTime = CLOSELEN - elevator->stateTime;//�޸ĵ�ǰ״̬ʱ��
			actionUsed = 1;
		}
		else if (actionTemp.reqAct == CLOSE){
			elevator->eleState = DOORCLOSING;//������ź��ٰ����ż� 
			actionUsed = 1;//��ǰ���ִ�У���������Ϊ1
		}
		else if (elevator->stateTime == 0){ //�Ѿ����� 
			if (actionTemp.reqAct != -1){ //��������� 
				if (actionTemp.reqF > elevator->nowF){ //���� 
					elevator->eleState = UP;//�޸ĵ���״̬
					elevator->goalF = actionTemp.reqF;//�޸ĵ��ݵ�Ŀ��¥��
					elevator->stateTime = ONELAYERLEN;//�޸ĵ�ǰ״̬ʱ��
				}
				else if (actionTemp.reqF == elevator->nowF){ //��¥����Ӧ 
					elevator->eleState = DOOROPENING;//�޸ĵ���״̬
					elevator->stateTime = OPENLEN;//�޸ĵ�ǰ״̬ʱ��
				}
				else if (actionTemp.reqF<elevator->nowF){  //�½� 
					elevator->eleState = DOWN;//�޸ĵ���״̬
					elevator->goalF = actionTemp.reqF;//�޸ĵ��ݵ�Ŀ��¥��
					elevator->stateTime = ONELAYERLEN;//�޸ĵ�ǰ״̬ʱ��
				}
				actionUsed = 1;//��ǰ���ִ�У���������Ϊ1
			}
			else
				elevator->eleState = IDLE;//���û��������������
		}
		else {
			elevator->stateTime--; //����δ����ʱ ��״̬ʱ���һ
		}
		break;
	case (UP) : //���� 
		if (elevator->stateTime != 0) //��λ������֮��ʱ 
			elevator->stateTime--;//�޸ĵ�ǰ״̬ʱ�䣬״̬ʱ���һ
		else if ( elevator->nowF < elevator->goalF ){ //��δ����ָ��¥��ʱ 
			elevator->nowF ++;//�޸ĵ��ݵ�ǰ¥��
			if ( elevator->nowF != elevator->goalF )//��ǰ¥�㻹�ǲ�ΪĿ��¥��ʱ
				elevator->stateTime = ONELAYERLEN;//�޸ĵ�ǰ״̬ʱ�䣬��������
		}
		else{ //������ָ��¥��ʱ 
			elevator->eleState = DOOROPENING;//�޸ĵ���״̬
			elevator->stateTime = OPENLEN;//�޸ĵ�ǰ״̬ʱ��
		}
		break;
	case (DOWN) : //���� 
		if (elevator->stateTime != 0)//��λ������֮��ʱ 
			elevator->stateTime--;//�޸ĵ�ǰ״̬ʱ�䣬״̬ʱ���һ
		else if (elevator->nowF > elevator->goalF){//��δ����ָ��¥��ʱ 
			elevator->nowF--;//�޸ĵ��ݵ�ǰ¥��
			if (elevator->nowF != elevator->goalF )//��ǰ¥�㻹�ǲ�ΪĿ��¥��ʱ
				elevator->stateTime = ONELAYERLEN;//�޸ĵ�ǰ״̬ʱ�䣬��������
		}
		else{
			elevator->eleState = DOOROPENING;//�޸ĵ���״̬
			elevator->stateTime = OPENLEN;//�޸ĵ�ǰ״̬ʱ��
		}
		break;
	}
	if (actionUsed == 1){ //�����ִ��ʱ��ɾȥ 
		(* reqTempPtr).reqAct = actionTemp.reqAct;//��ɾȥ������浽reqTempPtrָ��Ľṹ��
		(* reqTempPtr).reqF = actionTemp.reqF;
		deleteListNode(headp);//ɾ���ڵ�
	}
}

 /*ɾ���ڵ�*/
//listPtr��Ҫɾ���Ľڵ�ǰ��һ���ڵ�ĵ�ַ
void deleteListNode(LISTNODEPTR listPtr){
	LISTNODEPTR followPtr;
	followPtr = listPtr->nextPtr;
	listPtr->nextPtr = followPtr->nextPtr;
	free(followPtr);
}

/*�Ե��ݽ��г�ʼ��*/
void initElevator(struct ELEVATOR * elePtr){  
	elePtr->eleState = IDLE;
	elePtr->nowF = 1;
	elePtr->goalF = 1;
	elePtr->stopTime = 0;
	elePtr->stateTime = 0;
}

//�Կ������������Ӧ
//req==1���������� req==2:��������
void doorReqFunc(int req, struct ELEVATOR * elevatorPtr){
	if (req == 1){//��������
		if (elevatorPtr->eleState == STOP){//�ڵ���ͣ��״̬������������������һ��ͣ����ʱ��Ƭ
			elevatorPtr->stateTime = STOPLEN;
		}
		else if (elevatorPtr->eleState == DOORCLOSING){//�������ڹ������������������ѹص��ŵĻ����Ͽ���
			elevatorPtr->eleState = DOOROPENING;
			elevatorPtr->stateTime = CLOSELEN-elevatorPtr->stateTime;
		}
		else if (elevatorPtr->eleState == IDLE){//���ݴ��ڿ���״̬��ֱ����Ӧ��������
			elevatorPtr->eleState = DOOROPENING;
			elevatorPtr->stateTime = OPENLEN;
		}
	}
	else if (req == 2){//��������
		if (elevatorPtr->eleState == STOP){//�ڵ���ͣ��״̬������������ֱ����Ӧ��������
			elevatorPtr->eleState = DOORCLOSING;
			elevatorPtr->stateTime = CLOSELEN;
		}
		else if (elevatorPtr->eleState == DOOROPENING){//�������ڹ������������������ѿ����ŵĻ����Ϲ���
			elevatorPtr->eleState = DOORCLOSING;
			elevatorPtr->stateTime = elevatorPtr->stateTime;
		}
	}
}
//end of state.c