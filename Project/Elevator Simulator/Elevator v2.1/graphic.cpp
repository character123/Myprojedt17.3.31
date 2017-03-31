#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "graphics.h"
#include "types.h"
#include "prototypes.h"

extern HANDLE hMutex;

//graphic.c

DWORD WINAPI graphicOutProc(LPVOID lpParameter){
	//���岢��ʼ������
	struct HANDLETRANS * handleTransPtr = (struct HANDLETRANS *) lpParameter; //������������ʽ�� 
	LISTNODEPTR headPtr = (*handleTransPtr).headp;//��������
	struct ELEVATOR elevator =*( (*handleTransPtr).elevatorPtr);//����״̬
	int serviceMethod = *((*handleTransPtr).serviceMethodPtr);//����ģʽ
	struct REQ reqTemp = *((*handleTransPtr).reqTempPtr);//��ʱ����
	float fpscount=0;//֡����������
	int fpsclear = 0;//�Ƿ�fpscount����
	int insideReq[9] = { 0 };//�ڲ���������
	int outsideUpReq[9] = { 0 };//�ⲿ������������
	int outsideDownReq[9] = { 0 };//�ⲿ������������
	LISTNODEPTR currentPtr = headPtr;//������������ж�ȡ������Ӧ��������ʾ����
	struct ELEVATOR lastState=elevator;//ǰһ״̬
	
	//ͼƬ���ļ����ַ���
	char nameInOn[] = "pic\\button-1-on.jpg";
	char nameInOff[] = "pic\\button-2-off.jpg";
	char nameInOri[] = "pic\\button-1-ori.jpg";
	char nameFloor[] = "pic\\elevator-1.jpg";
	char * nameOut[6] = { "pic\\button-up-ori.jpg", "pic\\button-up-on.jpg", "pic\\button-up-off.jpg",
		"pic\\button-down-ori.jpg", "pic\\button-down-on.jpg", "pic\\button-down.jpg" };
	char nameNowFloor[] = "pic\\floor-1.bmp";

	//ͼƬ������
	int i;//��ʱ����
	int buttonInX = 576, buttonInY = 192;//�ڲ�����
	int buttonOutUpX = 192, buttonOutUpY = 96;//�ⲿ����
	int buttonOutDownX = 240, buttonOutDownY = 48;//�ⲿ����
	int buttonFloorX = 96, buttonFloorY = 48;//¥������
	int elevatorX = 96, elevatorY = 96;//����
	int serviceX = 336, serviceY = 288;//���Է���
	int nowFloorX = 624, nowFloorY = 144;//��ǰ¥��
	int goalFloorX = 480, goalFloorY = 144;//Ŀ��¥��
	int buttonDoorX = 624, buttonDoorY = 192;//������
	int doorX, doorY;//��
	int doorCoverX=elevatorX+48, doorCoverY;//�ŵĵ���
	int directionX = 432, directionY = 144;
	setinitmode(INIT_NOBORDER);
	initgraph(768, 672);//����ͼ�λ���

	//��ȡͼƬ��Դ
	PIMAGE imgBackground = newimage();//����ͼƬ
	getimage(imgBackground, "pic//background.jpg");
	
	PIMAGE imgFloor[9];//¥��
	for (i = 0; i < 9; i++){
		imgFloor[i] = newimage();
		nameFloor[13] = 8 - i + 49;
		getimage(imgFloor[i], nameFloor);
	}
	PIMAGE buttonInOri[9];//�ڲ�����
	for (i = 0; i < 9; i++){
		nameInOri[11] = i + 49;
		buttonInOri[i] = newimage();
		getimage(buttonInOri[i], nameInOri);
	}
	PIMAGE buttonInOn[9];//�ڲ���
	for (i = 0; i < 9; i++){
		nameInOn[11] = i + 49;
		buttonInOn[i] = newimage();
		getimage(buttonInOn[i], nameInOn);
	}
	PIMAGE buttonOutUpOn = newimage();//�ⲿ������
	getimage(buttonOutUpOn, nameOut[1]);
	PIMAGE buttonOutUpOri = newimage();//�ⲿ���ϲ���
	getimage(buttonOutUpOri, nameOut[0]);
	PIMAGE buttonOutDownOn = newimage();//�ⲿ������
	getimage(buttonOutDownOn, nameOut[4]);
	PIMAGE buttonOutDownOri = newimage();//�ⲿ���²���
	getimage(buttonOutDownOri, nameOut[3]);
	PIMAGE imgElevator = newimage();//����
	getimage(imgElevator, "pic\\elevator-inside.jpg");
	PIMAGE imgNowFloor[9];//��ǰ¥��
	for (i = 0; i < 9; i++){
		imgNowFloor[i] = newimage();
		nameNowFloor[10] = i + 49;
		getimage(imgNowFloor[i], nameNowFloor);
	}
	PIMAGE imgService1off = newimage();//����ģʽ
	getimage(imgService1off, "pic\\service-1-of.jpg");
	PIMAGE imgService1on = newimage();
	getimage(imgService1on, "pic\\service-1-on.jpg");
	PIMAGE imgService2off = newimage();
	getimage(imgService2off, "pic\\service-2-of.jpg");
	PIMAGE imgService2on = newimage();
	getimage(imgService2on, "pic\\service-2-on.jpg");
	PIMAGE imgService3on = newimage();
	getimage(imgService3on, "pic\\service-3-on.jpg");
	PIMAGE imgService3off = newimage();
	getimage(imgService3off, "pic\\service-3-off.jpg");
	PIMAGE imgButtonDoorOpen = newimage(); //���ż�
	getimage(imgButtonDoorOpen, "pic\\door-open.jpg");
	PIMAGE imgButtonDoorClose = newimage();//���ż�
	getimage(imgButtonDoorClose, "pic\\door-close.jpg");
	PIMAGE imgDoor = newimage();//��
	getimage(imgDoor, "pic\\elevator-door.jpg");
	PIMAGE imgDoorCoverRight = newimage();//���ŵ�
	getimage(imgDoorCoverRight, "pic\\elevator-doorcover-right.jpg");
	PIMAGE imgDoorCoverLeft = newimage();//���ŵ�
	getimage(imgDoorCoverLeft, "pic\\elevator-doorcover-left.jpg");
	PIMAGE imgDirectionUp = newimage();
	getimage(imgDirectionUp, "pic\\up.jpg");
	PIMAGE imgDirectionDown = newimage();
	getimage(imgDirectionDown, "pic\\down.jpg");
	PIMAGE imgDirectionOther = newimage();
	getimage(imgDirectionOther, "pic\\other.jpg");
	PIMAGE imgButtonOutDownOff = newimage();
	getimage(imgButtonOutDownOff, nameOut[5]);
	PIMAGE buttonOutUpOff = newimage();
	getimage(buttonOutUpOff, nameOut[2]);
	PIMAGE buttonNoUse = newimage();
	getimage(buttonNoUse, nameOut[2]);
	PIMAGE buttonInOff[4];//�ڲ�����
	for (i = 0; i < 9; i++){
		nameInOff[11] = i + 50;
		buttonInOff[i] = newimage();
		getimage(buttonInOff[i], nameInOff);
	}

	while (1){///��ʼͼ�λ������ѭ��
		headPtr = (*handleTransPtr).headp;//��ȡ��������
		elevator = *((*handleTransPtr).elevatorPtr);//��ȡ����״̬
		serviceMethod = *((*handleTransPtr).serviceMethodPtr);//��ȡ����ģʽ
		reqTemp.reqAct = (*((*handleTransPtr).reqTempPtr)).reqAct;//��ȡ��ʱ���������
		reqTemp.reqF = (*((*handleTransPtr).reqTempPtr)).reqF;//��ȡ��ʱ�����¥��
		currentPtr = headPtr;//������������׽ڵ�ҽ��ڵ�ǰ�ڵ���
		
		//������һ״̬�͵�ǰ״̬���ж��Ƿ�fpscount����
		if (lastState.eleState == STOP){
			if (elevator.eleState != STOP){
				fpscount = 0;
			}
		}
		else if (lastState.eleState == UP || lastState.eleState == DOWN){
			if (elevator.eleState == DOOROPENING || lastState.nowF!=elevator.nowF || elevator.nowF==elevator.goalF){
				fpscount = 0;
			}
		}
		else if (elevator.eleState == DOOROPENING){//
			if (elevator.stateTime == 3){
				if (fpsclear == 0){
					fpscount = 0;
					fpsclear = 1;
				}
			}
			else fpsclear = 0;
		}
		else if (lastState.eleState == DOORCLOSING){
			if (elevator.stateTime == 3){
				if (fpsclear == 0){
					fpscount = 0;
					fpsclear = 1;
				}
			}
			else fpsclear = 0;
		}
		else if (lastState.eleState == IDLE && elevator.eleState != IDLE){
			fpscount = 0;
		}
		lastState = elevator;
		
		//��������Ӧ������Ӧ��������
		while ((currentPtr->nextPtr) != NULL){
			currentPtr = currentPtr->nextPtr; //��ͷ��ʼ���в�ѯ��ÿһ�ζ����в�ѯ 
			if (currentPtr!=NULL)
			switch (currentPtr->reqData.reqAct){ //���������ڵ��ڲ����ⲿ���� 
				case (INSIDE) :
					insideReq[currentPtr->reqData.reqF - 1] = 1;
					break;
				case (OUTSIDEUP) :
					outsideUpReq[currentPtr->reqData.reqF - 1] = 1;
					break;
				case (OUTSIDEDOWN) :
					outsideDownReq[currentPtr->reqData.reqF - 1] = 1;
			}
		}
		switch (reqTemp.reqAct){ //���Ѿ�����ȡ��������Ӧ���������������
			case (INSIDE) :
				insideReq[reqTemp.reqF - 1] = 1;
				break;
			case (OUTSIDEUP) :
				outsideUpReq[reqTemp.reqF - 1] = 1;
				break;
			case (OUTSIDEDOWN) :
				outsideDownReq[reqTemp.reqF - 1] = 1;
		}

		cleardevice();//����
		
		//����Ӧͼ�����
		putimage(0, 0, imgBackground);
		for (i = 0; i < 8; i++){//�ⲿ����
			buttonOutUpY += 48;
			if (outsideUpReq[7-i] == 1){
				putimage(buttonOutUpX, buttonOutUpY, buttonOutUpOn);
			}
			else{
				putimage(buttonOutUpX, buttonOutUpY, buttonOutUpOri);
			}
			outsideUpReq[7-i] = 0;
		}
		buttonOutUpY = 96;
		for (i = 0; i < 8; i++){//�ⲿ����
			buttonOutDownY += 48;
			if (outsideDownReq[8-i] == 1){
				putimage(buttonOutDownX, buttonOutDownY, buttonOutDownOn);
			}
			else{
				putimage(buttonOutDownX, buttonOutDownY, buttonOutDownOri);
			}
			outsideDownReq[8-i] = 0;
		}
		buttonOutDownY = 48;
		for (i = 8; i >= 0; i--){//�ڲ�����
			buttonInY += 48;
			if (i == 4){
				buttonInX += 48;
				buttonInY = 336;
			}
			if (insideReq[i] == 0){
				putimage(buttonInX, buttonInY, buttonInOri[i]);
			}
			else{
				putimage(buttonInX, buttonInY, buttonInOn[i]);
			}
			insideReq[i] = 0;
		}
		buttonInX = 576;
		buttonInY = 192;
		for (i = 0; i < 9; i++){//¥������
			buttonFloorY += 48;
			putimage(buttonFloorX, buttonFloorY, imgFloor[i]);
		}
		buttonFloorY = 48;
		buttonDoorX -= 48;
		putimage(buttonDoorX, buttonDoorY, imgButtonDoorOpen);//���Ű�ť
		buttonDoorX += 48;//���Ű�ť
		putimage(buttonDoorX, buttonDoorY, imgButtonDoorClose);
		putimage(nowFloorX, nowFloorY, imgNowFloor[elevator.nowF - 1]);//��ǰ¥��
		putimage(goalFloorX, goalFloorY, imgNowFloor[elevator.goalF - 1]);//Ŀ��¥��
		if (serviceMethod == 1){
			putimage(serviceX, serviceY, imgService1on);//������ʾ
			putimage(serviceX, serviceY + 48, imgService2off);
			putimage(serviceX, serviceY + 96, imgService3off);
		}
		else if (serviceMethod == 2){
			putimage(serviceX, serviceY, imgService1off);
			putimage(serviceX, serviceY + 48, imgService2on);
			putimage(serviceX, serviceY + 96, imgService3off);
		}
		else{
			putimage(serviceX, serviceY, imgService1off);
			putimage(serviceX, serviceY + 48, imgService2off);
			putimage(serviceX, serviceY + 96, imgService3on);
		}
		if (elevator.eleState == UP){//����
			elevatorY = (9 - elevator.nowF) * 48 + 96 - (int)(fpscount / 63 * 60 / 1.25);
		}
		else if (elevator.eleState == DOWN){
			elevatorY = (9 - elevator.nowF) * 48 + 96 + (int)(fpscount / 63 * 60 / 1.25);
		}
		else {
			elevatorY = 96+ (9 - elevator.nowF) * 48;
		}
		putimage(elevatorX, elevatorY, imgElevator);
		doorY = elevatorY;//��������
		if (elevator.eleState == DOOROPENING){
			doorX = elevatorX - (int)(fpscount / 66 * 60 / 2.5);
		}
		else if (elevator.eleState == DOORCLOSING){
			doorX = elevatorX - 24 + (int)(fpscount / 66 * 60 / 2.5);
		}
		else if (elevator.eleState == STOP){
			doorX = elevatorX - 24;
		}
		else{
			doorX = elevatorX;
		}
		putimage(doorX, doorY, imgDoor);
		if (elevator.eleState == DOOROPENING){//��������
			doorX = elevatorX + 24 + (int)(fpscount / 66 * 60 / 2.5);
		}
		else if (elevator.eleState == DOORCLOSING){
			doorX = elevatorX + 48 - (int)(fpscount / 66 * 60 / 2.5);
		}
		else if (elevator.eleState == STOP){
			doorX = elevatorX + 48;
		}
		else {
			doorX = elevatorX + 24;
		}
		putimage(doorX, doorY, imgDoor);
		doorCoverX -= 72;
		doorCoverY = elevatorY;//�������ڵ�
		putimage(doorCoverX, doorCoverY, imgDoorCoverLeft);
		doorCoverX += 72;//�������ڵ�
		putimage(doorCoverX, doorCoverY, imgDoorCoverRight);
		if (elevator.eleState == UP){
			putimage(directionX, directionY, imgDirectionUp);
		}
		else if (elevator.eleState == DOWN){
			putimage(directionX, directionY, imgDirectionDown);
		}
		else {
			putimage(directionX, directionY, imgDirectionOther);
		}
		if (serviceMethod == 3){
			buttonOutUpY = 240;
			for (i = 0; i < 4; i++){//�ⲿ����
				buttonOutUpY += 48;
				putimage(buttonOutUpX, buttonOutUpY, buttonOutUpOff);
			}
			buttonOutUpY = 96;
			buttonOutDownY = 240;
			for (i = 0; i < 4; i++){//�ⲿ����
				buttonOutDownY += 48;
				putimage(buttonOutDownX, buttonOutDownY, imgButtonOutDownOff);
			}
			buttonOutDownY = 48;
			buttonInX = 624;
			buttonInY = 288;
			for (i = 3; i >= 0; i--){//�ڲ�����
				buttonInY += 48;
				putimage(buttonInX, buttonInY, buttonInOff[i]);
			}
			buttonInX = 576;
			buttonInY = 192;
		}

		fpscount+=1;//֡����һ
		delay_ms(50);//ÿ�����ˢ��20֡
	}//end of while
	
	//ɾ��ͼƬ����
	for (i = 0; i < 9; i++)
		delimage(imgNowFloor[i]);
	for (i = 0; i < 9; i++)
		delimage(imgFloor[i]);
	for (i = 0; i < 9; i++)
		delimage(buttonInOri[i]);
	for (i = 0; i < 9; i++)
		delimage(buttonInOn[i]);
	delimage(imgService1off);
	delimage(imgService1on);
	delimage(imgService2off);
	delimage(imgService2on);
	delimage(imgBackground);
	delimage(imgElevator);
	delimage(imgDoor);
	delimage(imgDoorCoverLeft);
	delimage(imgDoorCoverRight);
	delimage(buttonOutUpOn);
	delimage(buttonOutUpOri);
	delimage(buttonOutDownOn);
	delimage(buttonOutDownOri);
	delimage(imgButtonDoorOpen);
	delimage(imgButtonDoorClose);
	delimage(imgButtonOutDownOff);
	closegraph();//�ر�ͼ�λ���
	DWORD res = 1;//C++�ж���ķ���ֵ
	return res;
}