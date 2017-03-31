#include "stdafx.h"
#include "TFTP_server.h"
#include <stdlib.h>
#include <time.h>
#include<winsock2.h>
#pragma comment( lib, "Ws2_32.lib" )
using namespace std;

unsigned short int getPort();

UINT handleRRQThread(PVOID p) {
	unsigned short int state = IDLE;
	int reSendTimes = 0;
	unsigned short int sendSeqNo = 1;
	CStdioFile file;

	char dataBuf[DATA_BUF_LEN]; //���ڷ���data
	clear(dataBuf, DATA_BUF_LEN);

	char recvBuf[RECV_BUF_LEN];
	clear(recvBuf, RECV_BUF_LEN);
	memcpy(recvBuf, listenBuf, RECV_BUF_LEN);
	//��������ʵ�����ݴ����socket
	SOCKET commSock;
	struct sockaddr_in t_local, t_from;

	commSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (commSock == INVALID_SOCKET) {
		cout << "�����޷�Ϊ�̴߳��� Socket��" << endl << endl;
		return 0;
	}
	t_local.sin_family = AF_INET;
	t_local.sin_port = htons(getPort());  //1024-5000 ���ȡ
	t_local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	t_from.sin_family = ((sockaddr_in*)p)->sin_family;
	t_from.sin_port = ((sockaddr_in*)p)->sin_port;
	t_from.sin_addr.S_un.S_addr = ((sockaddr_in*)p)->sin_addr.S_un.S_addr;

	if (bind(commSock, (struct sockaddr*)&t_local, sizeof(t_local)) != 0) {
		cout << "�����߳��޷��� Socket��" << endl << endl;
		closesocket(commSock);
		return 0;
	}
	flag = false;  //����������������������û�������

	unsigned long argp = 65535;
	int ioc = ioctlsocket(commSock, FIONBIO, &argp);
	if (ioc == SOCKET_ERROR) {
		cout << "�����׽ӿ�����Ϊ������ģʽʧ�ܡ�" << endl << endl;
		closesocket(commSock);
		WSACleanup();
		return 0;
	}

	//������Ϣ��recvBuf��
	REQPacket RRQpacket = decode_REQPacket(recvBuf);
	state = IDLE;
	while (1) {
		switch (state) {
			case IDLE:
				state = s_IDLE(RRQpacket);
				break;
			case WAIT_R_RESP:
				state = s_WAIT_R_RESP(dataBuf, reSendTimes, sendSeqNo, &file, RRQpacket, commSock, t_from);
				break;
			case WAIT_ACK:
				state = s_WAIT_ACK(dataBuf, reSendTimes, sendSeqNo, &file, commSock, t_from);
				break;
			case WAIT_LAST_ACK:
				state = s_WAIT_LAST_ACK(dataBuf, reSendTimes, sendSeqNo, &file, commSock, t_from);
				break;
			case SUCCESS_OVER:
				cout << "��ʾ���� " << RRQpacket.mode << " ģʽ�����ļ� " << RRQpacket.fileName << "��" << endl << endl;
			default:
				cout << endl;
				closesocket(commSock);
				return 1;  //ERROR_OVER
		}
	}
}

UINT handleWRQThread(PVOID p) {
	unsigned short int state = IDLE;
	int reSendTimes = 0;
	unsigned short int recvSeqNo = 0;
	CStdioFile file;

	char recvBuf[RECV_BUF_LEN];
	clear(recvBuf, RECV_BUF_LEN);
	memcpy(recvBuf, listenBuf, RECV_BUF_LEN);
	//��������ʵ�����ݴ����socket
	SOCKET commSock;
	struct sockaddr_in t_local, t_from;
	commSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (commSock == INVALID_SOCKET) {
		cout << "�����޷�Ϊ�̴߳��� Socket��" << endl << endl;
		return 0;
	}
	t_local.sin_family = AF_INET;
	t_local.sin_port = htons(getPort());  //1024-5000 ���ȡ
	t_local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	t_from.sin_family = ((sockaddr_in*)p)->sin_family;
	t_from.sin_port = ((sockaddr_in*)p)->sin_port;
	t_from.sin_addr.S_un.S_addr = ((sockaddr_in*)p)->sin_addr.S_un.S_addr;

	if (bind(commSock, (struct sockaddr*)&t_local, sizeof(t_local)) != 0) {
		cout << "�����߳��޷��� Socket��" << endl << endl;
		closesocket(commSock);
		return 0;
	}
	flag = false;  //����������������������û�������

	unsigned long argp = 65535;
	int ioc = ioctlsocket(commSock, FIONBIO, &argp);
	if (ioc == SOCKET_ERROR) {
		cout << "����Socket ����Ϊ������ģʽʧ�ܡ�" << endl << endl;
		closesocket(commSock);
		WSACleanup();
		return 0;
	}

	//������Ϣ��recvBuf��
	REQPacket WRQpacket = decode_REQPacket(recvBuf);
	state = IDLE;
	while (1) {
		switch (state) {
			case IDLE:
				state = s_IDLE(WRQpacket);
				break;
			case WAIT_W_RESP:
				state = s_WAIT_W_RESP(reSendTimes, recvSeqNo, commSock, t_from);
				break;
			case WAIT_FIRST_P:
				state = s_WAIT_FIRST_P(reSendTimes, recvSeqNo, &file, WRQpacket, commSock, t_from);
				break;
			case WAIT_NEXT_P:
				state = s_WAIT_NEXT_P(reSendTimes, recvSeqNo, &file, commSock, t_from);
				break;
			case SUCCESS_OVER:
				closesocket(commSock);
				cout << "��ʾ���� " << WRQpacket.mode << " ģʽ�յ��ļ� " << WRQpacket.fileName << "��" << endl << endl;
				return 1;
			default:
				cout << endl;
				closesocket(commSock);
				return 1;  //ERROR_OVER
		}
	}
}

unsigned short int getPort() {
	srand(time(NULL));
	int port = 1024 + rand() % 3977; //���ڵõ�1024��5000�е���һ����
	return port;
}