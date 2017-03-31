#include <stdio.h>
#include <windows.h>
#include "graphics.h"
#include "types.h"
#include "prototypes.h" 

extern HANDLE hMutex;//���뻥�����

//strategy.c


//��������ݲ��Բ�ͬ��������������ĺ���λ�� 
// reqMode:����ģʽ��headPtr��ָ��洢��������� ͷ�ڵ㣬reqs���洢����Ľṹ���飬reqsSize��reqs����Ԫ�صĸ�����elevator�����ݽṹ������reqTempPtr����ʱ��ŵ�����ṹ
void addServList(int reqMode, LISTNODEPTR headPtr, struct REQ * reqs, int reqsSize,struct ELEVATOR * elevatorPtr, struct REQ * reqTempPtr)
{
	//���岢��ʼ������
	LISTNODEPTR currentPtr = headPtr->nextPtr, tempPtr = NULL, previousPtr = headPtr; //��ʼ������ 
	struct REQ reqTemp; //��ʱ�ṹ
	int count = 0;//����ڵ���� 
	int is_overlap=0;//�ж������Ƿ��ظ���0Ϊδ�ظ���1Ϊ�ظ� 
	//������������������������������
	switch (reqMode){
		case (1) ://�����ȷ������
			while (count<reqsSize){//��δ���������������е�������������У�����ѭ��
				if (reqs[count].reqAct != -1){//������Ϊ��ʱ
					tempPtr = (LISTNODEPTR)malloc(sizeof(LISTNODE)); //�������ڴ�
					if (tempPtr != NULL){ //������
						tempPtr->reqData = reqs[count];//�ѵ�i�����󸳸�tempPtrָ��Ľṹ 
						if (reqs[count].reqAct == OPEN || reqs[count].reqAct == CLOSE){//����ǿ��Ź���ָ�� 
							if (reqs[count].reqAct == OPEN)//��������
								doorReqFunc(1, elevatorPtr);
							else//��������
								doorReqFunc(2, elevatorPtr);
						}//OPEN��& CLOSE
						else{//�鿴�Ƿ��������������ظ��Ľڵ� 
							LISTNODEPTR lastPtr = headPtr;
							LISTNODEPTR nowPtr = headPtr->nextPtr;//��ʼ��
							while (nowPtr != NULL){
								if (nowPtr->reqData.reqAct == tempPtr->reqData.reqAct
									&& nowPtr->reqData.reqF == tempPtr->reqData.reqF){//������ظ����� 
									is_overlap = 1;//�������ظ��������
									LISTNODEPTR T = nowPtr;//�ݴ��ظ�����Ľڵ� 
									lastPtr->nextPtr = T->nextPtr;
									free(T);//�ͷ��ظ�����Ľڵ� 
									break;
								}
								else{//�˽ڵ㲻�ظ�
									lastPtr = nowPtr;//ָ����� 
									nowPtr = nowPtr->nextPtr;
								}
							}
							if (is_overlap == 0){//�������������û���ظ��ڵ�
								if (reqs[count].reqAct == INSIDE){//�ڲ���������
									tempPtr->reqData = reqs[count];//�ѵ�i�����󸳸�tempPtrָ��Ľṹ 
									if (headPtr->nextPtr == NULL){//������û������ʱ ֱ�ӽ��� 
										headPtr->nextPtr = tempPtr;
										tempPtr->nextPtr = NULL;//tempPtrָ����¸��ڵ�Ϊ�� 
									}
									else{//������������
										while (currentPtr != NULL&&currentPtr->reqData.reqAct != OUTSIDEUP
											&&currentPtr->reqData.reqAct != OUTSIDEDOWN){
											//currentPtr��previousPtr���������ƶ�
											previousPtr = currentPtr;
											currentPtr = currentPtr->nextPtr; 
										}
										tempPtr->nextPtr = currentPtr;//�ݴ�ڵ㼴�ڲ����������ⲿ����֮ǰ 
										previousPtr->nextPtr = tempPtr;
									}
								}
								else{ //�ⲿ����
									tempPtr->nextPtr = NULL;//��ֹ���ʵ�ַ����
									if (currentPtr != NULL){
										while ((currentPtr->nextPtr) != NULL){//��ָ���Ƶ�����β�� 
											currentPtr = currentPtr->nextPtr;
										}
										currentPtr->nextPtr = tempPtr;//��������������β�� 
										currentPtr = tempPtr;// currentPtrָ������β�� 
									}
									else { //���ͷ�������ſ�ָ�� 
										headPtr->nextPtr = tempPtr;
										currentPtr = tempPtr;
									}
								}//else
							}//if
						}//else
						reqs[count].reqAct = EMPTY;
						is_overlap = 0;
					}
					else
						printf("Error when asking for space.\n"); //��ʾ���� 
				}
				count++;//������ȡ����������һ������
			}
			//�����ڲ�����
			break;
		
		case (2) ://˳��������
			while (count<reqsSize){//��δ���������������е�������������У�����ѭ��
				if (reqs[count].reqAct != -1){//������Ϊ��ʱ
					tempPtr = (LISTNODEPTR)malloc(sizeof(LISTNODE)); //�������ڴ�
					if (tempPtr != NULL){ //������
						tempPtr->reqData = reqs[count];//�ѵ�i�����󸳸�tempPtrָ��Ľṹ 
						if (reqs[count].reqAct == OPEN || reqs[count].reqAct == CLOSE){//����ǿ��Ź���ָ�� 
							if (reqs[count].reqAct == OPEN)//��������
								doorReqFunc(1, elevatorPtr);
							else//��������
								doorReqFunc(2, elevatorPtr);
						}//OPEN��& CLOSE
						else{//������ǿ���������
								//�鿴�Ƿ��������������ظ��Ľڵ� 
								LISTNODEPTR lastPtr = headPtr;//��ʼ��
								LISTNODEPTR nowPtr = headPtr->nextPtr;
								while (nowPtr != NULL){
									if (nowPtr->reqData.reqAct == tempPtr->reqData.reqAct
										&& nowPtr->reqData.reqF == tempPtr->reqData.reqF){//������ظ����� 
										is_overlap = 1;//�������ظ��������
										LISTNODEPTR T = nowPtr;//�ݴ��ظ�����Ľڵ� 
										lastPtr->nextPtr = T->nextPtr;
										free(T);//�ͷ��ظ�����Ľڵ� 
										break;
									}
									else{
										lastPtr = nowPtr;//ָ����� 
										nowPtr = nowPtr->nextPtr;
									}
								}
								if (is_overlap == 0){//������ظ�
									if ((*elevatorPtr).eleState == UP){ //�ڵ�������ʱ���������һ�����ڵ�ǰĿ��¥������ϻ��ڲ��������ʱ���Զ���Ŀ��¥����������л�
										if (((tempPtr->reqData).reqAct == OUTSIDEUP || (tempPtr->reqData).reqAct == INSIDE) &&
											(*elevatorPtr).goalF > (tempPtr->reqData).reqF && (*elevatorPtr).nowF < (tempPtr->reqData).reqF){//�ж�����
											//��Ŀ��¥���ЧΪһ������ṹ��Ȼ�󽫴��������¼�����������滻
											reqTemp.reqF = (*elevatorPtr).goalF;
											(*elevatorPtr).goalF = (tempPtr->reqData).reqF;
											(tempPtr->reqData).reqF = reqTemp.reqF;
											(*reqTempPtr).reqF = (*elevatorPtr).goalF;
											reqTemp.reqAct = (*reqTempPtr).reqAct;
											(*reqTempPtr).reqAct = (tempPtr->reqData).reqAct;
											(tempPtr->reqData).reqAct = reqTemp.reqAct;
										}
									}

									else if ((*elevatorPtr).eleState == DOWN){//�ڵ����½�ʱ���������һ�����ڵ�ǰĿ��¥������»��ڲ��������ʱ���Զ���Ŀ��¥����������л� 
										if (((tempPtr->reqData).reqAct == OUTSIDEDOWN || (tempPtr->reqData).reqAct == INSIDE) &&
											(*elevatorPtr).goalF < (tempPtr->reqData).reqF && (*elevatorPtr).nowF >(tempPtr->reqData).reqF){//�ж�����
											//��Ŀ��¥���ЧΪһ������ṹ��Ȼ�󽫴��������¼�����������滻
											reqTemp.reqF = (*elevatorPtr).goalF;
											(*elevatorPtr).goalF = (tempPtr->reqData).reqF;
											(tempPtr->reqData).reqF = reqTemp.reqF;
											(*reqTempPtr).reqF = (*elevatorPtr).goalF;
											reqTemp.reqAct = (*reqTempPtr).reqAct;
											(*reqTempPtr).reqAct = (tempPtr->reqData).reqAct;
											(tempPtr->reqData).reqAct = reqTemp.reqAct;
										}
									}

									tempPtr->nextPtr = NULL;//�������ʱֱ�ӽ������������β������sortServList���д���
									if (currentPtr != NULL){
										while ((currentPtr->nextPtr) != NULL){//��ָ���Ƶ�����β�� 
											currentPtr = currentPtr->nextPtr;
										}
										currentPtr->nextPtr = tempPtr;//��������������β�� 
										currentPtr = tempPtr;// currentPtrָ������β�� 
									}
									else { //���ͷ�������ſ�ָ�� 
										headPtr->nextPtr = tempPtr;
										currentPtr = tempPtr;
									}
								}//�ж��ظ��Ĳ��ֽ���
							}//��Ҫ���ֽ���
						reqs[count].reqAct = EMPTY;//���������reqs�����
						is_overlap = 0;
					}
					else
						printf("Error when asking for space.\n"); //��ʾ���� 
				}///�ж�����Ϊ�յĲ��ֽ���
				count++;
			}//end of while
			break;
		case(3) ://�긣���ݲ���
			while (count<reqsSize){
				if (reqs[count].reqAct != -1){//������Ϊ��ʱ
					tempPtr = (LISTNODEPTR)malloc(sizeof(LISTNODE)); //�������ڴ�
					if (tempPtr != NULL){ //������
						tempPtr->reqData = reqs[count];//�ѵ�i�����󸳸�tempPtrָ��Ľṹ 
						if (reqs[count].reqAct == OPEN || reqs[count].reqAct == CLOSE){//����ǿ��Ź���ָ�� 
							if (reqs[count].reqAct == OPEN)//��������
								doorReqFunc(1, elevatorPtr);
							else//��������
								doorReqFunc(2, elevatorPtr);
						}//OPEN��& CLOSE
						else if (reqs[count].reqF<2 || reqs[count].reqF>5){//�긣������Ҫ�㣬���ϵ���ģʽ
								//������ǿ���������
								//�鿴�Ƿ��������������ظ��Ľڵ� 
								LISTNODEPTR lastPtr = headPtr;//��ʼ��
								LISTNODEPTR nowPtr = headPtr->nextPtr;
								while (nowPtr != NULL){
									if (nowPtr->reqData.reqAct == tempPtr->reqData.reqAct
										&& nowPtr->reqData.reqF == tempPtr->reqData.reqF){//������ظ����� 
										is_overlap = 1;//�������ظ��ڵ�
										LISTNODEPTR T = nowPtr;//�ݴ��ظ�����Ľڵ� 
										lastPtr->nextPtr = T->nextPtr;
										free(T);//�ͷ��ظ�����Ľڵ� 
										break;
									}
									else{
										lastPtr = nowPtr;//ָ����� 
										nowPtr = nowPtr->nextPtr;
									}
								}
								if (is_overlap == 0){//������ظ�
									if ((*elevatorPtr).eleState == UP){ //�ڵ�������ʱ���������һ�����ڵ�ǰĿ��¥������ϻ��ڲ��������ʱ���Զ���Ŀ��¥����������л�
										if (((tempPtr->reqData).reqAct == OUTSIDEUP || (tempPtr->reqData).reqAct == INSIDE) &&
											(*elevatorPtr).goalF > (tempPtr->reqData).reqF && (*elevatorPtr).nowF < (tempPtr->reqData).reqF){//�ж�����
											reqTemp.reqF = (*elevatorPtr).goalF;//��Ŀ��¥���ЧΪһ������ṹ��Ȼ�󽫴��������¼�����������滻
											(*elevatorPtr).goalF = (tempPtr->reqData).reqF;
											(tempPtr->reqData).reqF = reqTemp.reqF;
											(*reqTempPtr).reqF = (*elevatorPtr).goalF;
											reqTemp.reqAct = (*reqTempPtr).reqAct;
											(*reqTempPtr).reqAct = (tempPtr->reqData).reqAct;
											(tempPtr->reqData).reqAct = reqTemp.reqAct;
										}
									}

									else if ((*elevatorPtr).eleState == DOWN){//�ڵ����½�ʱ���������һ�����ڵ�ǰĿ��¥������»��ڲ��������ʱ���Զ���Ŀ��¥����������л� 
										if (((tempPtr->reqData).reqAct == OUTSIDEDOWN || (tempPtr->reqData).reqAct == INSIDE) &&
											(*elevatorPtr).goalF < (tempPtr->reqData).reqF && (*elevatorPtr).nowF >(tempPtr->reqData).reqF){//�ж�����
											reqTemp.reqF = (*elevatorPtr).goalF;//��Ŀ��¥���ЧΪһ������ṹ��Ȼ�󽫴��������¼�����������滻
											(*elevatorPtr).goalF = (tempPtr->reqData).reqF;
											(tempPtr->reqData).reqF = reqTemp.reqF;
											(*reqTempPtr).reqF = (*elevatorPtr).goalF;
											reqTemp.reqAct = (*reqTempPtr).reqAct;
											(*reqTempPtr).reqAct = (tempPtr->reqData).reqAct;
											(tempPtr->reqData).reqAct = reqTemp.reqAct;
										}
									}

									tempPtr->nextPtr = NULL;//�������ʱֱ�ӽ������������β������sortServList���д���
									if (currentPtr != NULL){
										while ((currentPtr->nextPtr) != NULL){//��ָ���Ƶ�����β�� 
											currentPtr = currentPtr->nextPtr;
										}
										currentPtr->nextPtr = tempPtr;//��������������β�� 
										currentPtr = tempPtr;// currentPtrָ������β�� 
									}
									else { //���ͷ�������ſ�ָ�� 
										headPtr->nextPtr = tempPtr;
										currentPtr = tempPtr;
									}
								}//�ж��ظ��Ĳ��ֽ���
							}//��Ҫ���ֽ���
						reqs[count].reqAct = EMPTY;//���������reqs�����
						is_overlap = 0;
					}
					else
						printf("Error when asking for space.\n"); //��ʾ���� 
				}///�ж�����Ϊ�յĲ��ֽ���
				count++;
			}//end of while
	}//end of switch 
}


//Ϊ˳���������õĲ��Խ��̣�������һ��Ŀ��¥��ʱ�˽��̻����в�ȷ����һ��Ŀ��¥�㣬��ȷ��Ŀ��¥��ʱ������������������λ
DWORD WINAPI addServListProc (LPVOID lpParameter){
	//���岢��ʼ������
	struct HANDLETRANS * handleTransPtr = (struct HANDLETRANS *) lpParameter; //������ĵ�ַ�ṹ������ʽ��
		int * serviceMethodPtr =  (* handleTransPtr).serviceMethodPtr;//�������ģʽ����
		LISTNODEPTR headp = (* handleTransPtr).headp;//������������ͷ���
		struct ELEVATOR * elevatorPtr= (* handleTransPtr).elevatorPtr; //�������״̬�Ľṹ������ַ
		int res = 0;//�ж��Ƿ���й�˳�����������ڲ��������� 
	
	while (1){//ѭ������
		if ((*elevatorPtr).eleState == UP || (*elevatorPtr).eleState == DOWN){//������״̬Ϊ�������½��ҵ����յ�ʱ
			if ((*elevatorPtr).stateTime != 0 && res == 1){//������й�������stateTime!=0 
				res = 0;
			}
			if (res == 0 && (*elevatorPtr).nowF == (*elevatorPtr).goalF && (*elevatorPtr).stateTime == 0){
				deleteInsideReq(headp, elevatorPtr);
				if ((*serviceMethodPtr) == 2 || (*serviceMethodPtr) == 3){//����˳�����ʱִ��
					if ((*elevatorPtr).goalF == 9){//������������Ͳ�ʱ����״̬��ת
						(*elevatorPtr).eleState = DOWN;
					}
					else if ((*elevatorPtr).goalF == 1){
						(*elevatorPtr).eleState = UP;
					}
					WaitForSingleObject(hMutex, INFINITE);//���뻥�����
					deleteInsideReq(headp, elevatorPtr);//�ж��Ƿ����ڲ�������Ҫɾ����������ɾȥ
					sortServList(headp, elevatorPtr);//�������з��ϲ��Ե�һ��������ǰ
					res = 1;
					ReleaseMutex(hMutex); //�ͷŻ������
				}//end of ˳�����
			}
		}
		Sleep(500);//ÿ0.5����һ��
	}//end of while(1)
}

void sortServList(LISTNODEPTR headp,struct ELEVATOR * elevatorPtr){
	//���岢��ʼ������
	LISTNODEPTR currentPtr = headp->nextPtr, tempPtr = NULL; //��ʼ������
	LISTNODEPTR lastPtr=headp,sortLastPtr=headp,sortCurrentPtr=headp;//��ʼ�������õ�����ڵ�
	struct REQ reqTemp;//��ʱ����
	reqTemp.reqF=(*elevatorPtr).nowF;//��ʼ��
	int sortRes=0;//�Ƿ��Ź���
	//����ɸѡ
	if (currentPtr!=NULL && currentPtr->nextPtr!=NULL){//������Ϊ��ʱ
		if ((*elevatorPtr).eleState==UP){//������ݴ˿�Ϊ����
			reqTemp.reqAct=OUTSIDEUP;//��ʱ�����״̬Ϊ����
			for (reqTemp.reqF = 9; currentPtr!=NULL; lastPtr=currentPtr,currentPtr=currentPtr->nextPtr){
				//����ȵ�ǰ��ߵ�¥�������Ϻ��л���Ŀ�꣬��ô����͵ĸ��ڵ�ǰ¥��������Ϻ��л���Ŀ���¥��ΪĿ��
				if ((currentPtr->reqData).reqAct==OUTSIDEUP || (currentPtr->reqData).reqAct==INSIDE){
					if ((currentPtr->reqData).reqF>(*elevatorPtr).nowF && (currentPtr->reqData).reqF<reqTemp.reqF){
						//��¼Ŀǰ����Ŀ��¥��Ľڵ��ַ
						sortLastPtr=lastPtr;
						sortCurrentPtr=currentPtr;
						sortRes=1;
						reqTemp.reqF=(currentPtr->reqData).reqF;//������ʱ����Ŀ��¥�㣬�����´�ѭ���Ƚ�
					}//end of if
				}//end of if
			}
			if(sortRes!=1)
				for (reqTemp.reqF = 1, currentPtr = headp->nextPtr, lastPtr = headp;
					currentPtr != NULL; lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
					//����޷�ȷ��Ŀ��¥�㣬��ô����ߵ����º��л���Ŀ������¥��Ϊ���ݵ�ǰĿ��¥��
					if ((currentPtr->reqData).reqAct==OUTSIDEDOWN || (currentPtr->reqData).reqAct==INSIDE){
						if ((currentPtr->reqData).reqF>reqTemp.reqF){
							//��¼Ŀǰ����Ŀ��¥��Ľڵ��ַ
							sortLastPtr=lastPtr;
							sortCurrentPtr=currentPtr;
							sortRes=1;
							reqTemp.reqF=(currentPtr->reqData).reqF;//������ʱ����Ŀ��¥�㣬�����´�ѭ���Ƚ�
						}
					}
				}//end of for
			if(sortRes!=1)
				for (reqTemp.reqF = (*elevatorPtr).nowF, currentPtr = headp->nextPtr, lastPtr = headp;
					currentPtr != NULL; lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
					//����޷�ȷ��Ŀ��¥�㣬��ô����͵����Ϻ�������¥��Ϊ���ݵ�ǰ��Ŀ��¥�㡣
					if ((currentPtr->reqData).reqAct==OUTSIDEUP){
						if ((currentPtr->reqData).reqF<reqTemp.reqF){
							//��¼Ŀǰ����Ŀ��¥��Ľڵ��ַ
							sortLastPtr=lastPtr;
							sortCurrentPtr=currentPtr;
							sortRes=1;
							reqTemp.reqF=(currentPtr->reqData).reqF;//������ʱ����Ŀ��¥�㣬�����´�ѭ���Ƚ�
						}
					}
				}//end of for
		}///end of if(up)
		if ((*elevatorPtr).eleState==DOWN){//������ݴ˿�Ϊ����
			reqTemp.reqAct=OUTSIDEDOWN;//��ʱ�����״̬Ϊ����
			for (reqTemp.reqF = 1, currentPtr = headp->nextPtr, lastPtr = headp; currentPtr != NULL;
				lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
				//����ȵ�ǰ��͵�¥�������º��л���Ŀ�꣬��ô����ߵĵ��ڵ�ǰ¥��������º��л���Ŀ���¥��ΪĿ��
				if ((currentPtr->reqData).reqAct==OUTSIDEDOWN || (currentPtr->reqData).reqAct==INSIDE){
					if ((currentPtr->reqData).reqF<(*elevatorPtr).nowF && (currentPtr->reqData).reqF>reqTemp.reqF){
						//��¼Ŀǰ����Ŀ��¥��Ľڵ��ַ
						sortLastPtr=lastPtr;
						sortCurrentPtr=currentPtr;
						sortRes=1;
						reqTemp.reqF=(currentPtr->reqData).reqF;//������ʱ����Ŀ��¥�㣬�����´�ѭ���Ƚ�
					}
				}
			}//end of for
			if(sortRes!=1)
			for (reqTemp.reqF = 9, currentPtr = headp->nextPtr, lastPtr = headp; currentPtr != NULL;
					lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
					//����޷�ȷ��Ŀ��¥�㣬��ô����͵����Ϻ��л���Ŀ������¥��Ϊ���ݵ�ǰĿ��¥��
					if ((currentPtr->reqData).reqAct==OUTSIDEUP || (currentPtr->reqData).reqAct==INSIDE){
						if ((currentPtr->reqData).reqF<reqTemp.reqF){
							//��¼Ŀǰ����Ŀ��¥��Ľڵ��ַ
							sortLastPtr=lastPtr;
							sortCurrentPtr=currentPtr;
							sortRes=1;
							reqTemp.reqF=(currentPtr->reqData).reqF;//������ʱ����Ŀ��¥�㣬�����´�ѭ���Ƚ�
						}
					}
				}//end of for 
			if(sortRes!=1)
				for (reqTemp.reqF = (*elevatorPtr).nowF, currentPtr = headp->nextPtr, lastPtr = headp;
					currentPtr != NULL; lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
					//����޷�ȷ��Ŀ��¥�㣬��ô����ߵ����º���¥��ΪĿ��¥��
					if ((currentPtr->reqData).reqAct=OUTSIDEDOWN){
						if ((currentPtr->reqData).reqF>reqTemp.reqF){
							//��¼Ŀǰ����Ŀ��¥��Ľڵ��ַ
							sortLastPtr=lastPtr;
							sortCurrentPtr=currentPtr;
							sortRes=1;
							reqTemp.reqF=(currentPtr->reqData).reqF;//������ʱ����Ŀ��¥�㣬�����´�ѭ���Ƚ�
						}
					}
				}//end of for
		}//end of if(down)
		if(sortRes==1){//��Ҫ������ǰʱ�����ʣ��Ľڵ㽻������
			tempPtr = headp->nextPtr;
			headp->nextPtr = sortCurrentPtr;
			sortLastPtr->nextPtr = tempPtr;
			tempPtr = (sortLastPtr->nextPtr)->nextPtr;
			(sortLastPtr->nextPtr)->nextPtr = sortCurrentPtr->nextPtr;
			sortCurrentPtr->nextPtr = tempPtr;
		}
	}
}

//ɾ���ڲ�����ڵ�
void deleteInsideReq(LISTNODEPTR headPtr, struct ELEVATOR * elevatorPtr){
	LISTNODEPTR currentPtr = headPtr->nextPtr, lastPtr = headPtr;//��ʼ��
	while (currentPtr != NULL){
		if ((currentPtr->reqData).reqAct == INSIDE && (currentPtr->reqData).reqF == (*elevatorPtr).nowF){
			//���ݵ���ĳ¥�㣬��¥�����ڲ�������ɾȥ�ڲ�����
			lastPtr->nextPtr = currentPtr->nextPtr;
			free(currentPtr);
			currentPtr = lastPtr->nextPtr;
		}
		else
			currentPtr = currentPtr->nextPtr;//�Ƶ��¸��ڵ������Ѱ
	}
}

//end of strategy.c
