#include<winsock2.h>
#pragma comment( lib, "Ws2_32.lib" )
#include "stdafx.h"
#include "TFTP_server.h"
using namespace std;

bool flag = true;
char listenBuf[RECV_BUF_LEN];

void startUp(void) {
	SOCKET listenSock = INVALID_SOCKET;
	struct sockaddr_in local, from;
	WSADATA wsadata;

	cout << "=============================" << endl;
	cout << "=                           =" << endl;
	cout << "=   TFTP Э�� - ��������    =" << endl;
	cout << "=                           =" << endl;
	cout << "=============================" << endl << endl;
	cout << endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		cout << "�����޷����� Winsock ����" << endl << endl;
		exit(0);
	}

	cout << "��ʾ��Winsock ��������" << endl << endl;

	listenSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listenSock == INVALID_SOCKET) {
		cout << "�����޷�Ϊ�����˿ڴ��� Socket ���ӡ�" << endl << endl;
		exit(0);
	}

	local.sin_family = AF_INET;
	local.sin_port = htons(69);
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(listenSock, (struct sockaddr*)&local, sizeof(local)) != 0) {
		cout << "�����޷������˿ڡ�" << endl << endl;
		closesocket(listenSock);
		exit(0);
	}

	cout << "��ʾ�����ڼ����˿� 69��" << endl << endl;

	int fromLen = sizeof(from);

	while (1) {
		flag = true;
		clear(listenBuf, RECV_BUF_LEN);
		recvfrom(listenSock, listenBuf, RECV_BUF_LEN, 0, (struct sockaddr*)&from, &fromLen);
		switch (getOpCode(listenBuf)) {
			case RRQ:
				cout << "��ʾ��������Ϊ RRQ��" << endl << endl;
				AfxBeginThread(handleRRQThread, &from, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
				break;
			case WRQ:
				cout << "��ʾ��������Ϊ WRQ��" << endl << endl;
				AfxBeginThread(handleWRQThread, &from, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
				break;
			default:
				flag = false;
				break;
		}
		while (flag) {
		}
	}
}



