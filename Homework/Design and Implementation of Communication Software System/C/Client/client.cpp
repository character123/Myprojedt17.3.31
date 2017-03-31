// client.cpp : ������������

#include "stdafx.h"
#include "client.h"
#include "comm.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_nState = IDLE;
int g_nEvent = 0;
int g_nSendTimes = 0;  //���ݰ��ط����������Ϊ3��
unsigned short int g_nSendSeqNo = 0;
unsigned short int g_nRecvSeqNo = 0;

char g_ptr[512];
char g_sFileName[50];
int  g_nLength = 0;
BOOL err = FALSE;

int T1 = 2;
int T2 = 1;
int T3 = 1;
int T4 = 1;

int T1ELAPSE = 2000;
int T2ELAPSE = 1000;
int T3ELAPSE = 1000;
int T4ELAPSE = 1000;

unsigned short int Type = 0;  //ACK,DATA,RRQ,WRQ,ERR,ReadFile_CONF,WriteFile_CONF

BOOL g_nconnect = TRUE;   //��ʾ���������ͨ���

int  g_nport = 69;  //��¼�������ͨ�ŵ�Ŀ�Ķ˿ں�,���ʹ�ö˿ں�69

CWinApp theApp;

using namespace std;

BOOL SendUser(unsigned short int nType, BOOL nerr);
BOOL GetFromUser();

CInitSock initSock;	// ��ʼ��Winsock��
SOCKET clientSock;
struct sockaddr_in local, from;
int fromLen = sizeof(from);

// main ����

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
	int nRetCode = 0;

	// ��ʼ��MFC������д�������ʾ
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		cerr << _T("���󣺳�ʼ��MFCʧ�ܡ�") << endl << endl;
		nRetCode = 1;
	}
	else {
		// ����
		memset(g_ptr, '\0', 512);
		memset(g_sFileName, '\0', 50);
		memset(g_TXBuf, '\0', 516);
		memset(g_RXBuf, '\0', 516);

		// �����׽���
		//SOCKET clientSock;
		//struct sockaddr_in local,from;
		WSADATA wsadata;

		unsigned short port = 3500;
		char ip[16];
		char ch;         //mode type

		memset(ip, 0, sizeof(ip));
		cout << "============================" << endl;
		cout << "=                          =" << endl;
		cout << "=    TFTP Э�� - �ͻ���    =" << endl;
		cout << "=                          =" << endl;
		cout << "============================" << endl << endl;
		cout << "�������������ip��ַ: ";
		cin >> ip;
		cout << endl << "�������Ķ˿ں�: 69" << endl << endl;
		//cout << "��ѡ����ģʽ: 1. netascii | NETASCII    2 .binary | BINARY \n";
		//cin >> ch;

		//if (ch == '1')
		Mode = "netascii";
		//if (ch == '2')
		//	Mode = "octet";

		if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
			cout << "�����޷����� Winsock ����" << endl << endl;
			return 0;
		}

		cout << "��ʾ��Winsock ������������" << endl << endl;

		from.sin_family = AF_INET;
		from.sin_port = htons(69);
		from.sin_addr.S_un.S_addr = inet_addr(ip);  //inet_addrֱ�ӷ��������ֽ���

		clientSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (clientSock == INVALID_SOCKET) {
			printf("���󣺳�ʼ�� Socket ʧ�� %d��\n\n", ::WSAGetLastError());
			return 0;
		}
		
		cout << "��ʾ���˿� 69 ����ʹ�á�" << endl << endl;

		local.sin_family = AF_INET;
		local.sin_port = htons(port);
		local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		if (bind(clientSock, (struct sockaddr*)&local, sizeof(local)) != 0) {
			cout << "���󣺶˿� 69 �޷�ʹ�á�" << endl << endl;
			closesocket(clientSock);
			WSACleanup();
			return 0;
		}

		unsigned long argp = 65535;
		int ioc = ioctlsocket(clientSock, FIONBIO, &argp);
		if (ioc == SOCKET_ERROR) {
			cout << "�����׽ӿ�����Ϊ������ģʽʧ�ܡ�" << endl << endl;
			closesocket(clientSock);
			WSACleanup();
			return 0;
		}

		GetFromUser();      //�����û������ļ�������
		while (g_nconnect) {
			switch (g_nState) {
				case IDLE:
					TFTP_IDLE();
					break;
				case WAIT_FIRST_P:
					TFTP_WAIT_FIRST_P();
					break;
				case WAIT_NEXT_P:
					TFTP_WAIT_NEXT_P();
					break;
				case WAIT_ACK:
					TFTP_WAIT_ACK();
					break;
				case WAIT_LAST_ACK:
					TFTP_WAIT_LAST_ACK();
					break;
				default:
					g_nconnect = false;
			}
		}
		// �ر�Socket
		closesocket(clientSock);
	}
	system("PAUSE");
	return nRetCode;
}


BOOL TFTP_IDLE() {  
	//��Type�ж��Ƿ�RRQ����WRQ����������ҷ��ͺ������һ��״̬
	cout << "----------------------------" << endl;
	cout << "�ͻ���״̬�����С�" << endl << endl;
	int send = 0;
	if (g_nEvent == 0) {
		g_nconnect = false;
		return FALSE;
	}
	switch (g_nEvent) {
		case ReadFile_REQ:
			if (isFileExist(g_sFileName)) {
				// ��������ͬ���ļ�ʱ��������
				cout << "��ʾ���ڱ��ش���ͬ���ļ�����ֹͣ���ء�" << endl << endl;
				g_nState = IDLE;
				g_nconnect = false;
				return FALSE;
			}
			else {
				g_nRecvSeqNo = 1;
				Type = RRQ;
				SendPacket(Type);
				send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, sizeof(from));
				g_nState = WAIT_FIRST_P;
				g_nconnect = true;
			}
			break;

		case WriteFile_REQ:
			if (isFileExist(g_sFileName)) {
				// ������������ļ�ʱ���ϴ��ļ�
				g_nSendSeqNo = 0;
				Type = WRQ;
				SendPacket(Type);
				send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, sizeof(from));
				g_nState = WAIT_ACK;
				g_nconnect = true;
			}
			else {
				cout << "�����ϴ��ļ������ڡ�" << endl << endl;
				g_nState = IDLE;
				g_nconnect = false;
				return FALSE;
			}
			break;

		default:
			break;
	}
	g_nEvent = 0;
	return TRUE;
}

BOOL TFTP_WAIT_FIRST_P() {
	//����RRQ��ȴ���һ���ĵ���
	cout << "----------------------------" << endl;
	cout << "�ͻ���״̬���ȴ���һ�����ݰ����" << endl << endl;
	int send = 0;
	int ret = 0;
	BOOL recv = false;

	// ������ʱ�����ȴ���һ��DATA�ĵ���
	// setsockopt(clientSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&T1ELAPSE,sizeof(int));
	for (g_nSendTimes = 0; g_nSendTimes<MAX; g_nSendTimes++) {
		// ret=recvfrom(clientSock,g_RXBuf,516,0,(struct sockaddr*)&from,&fromLen);
		ret = recvd(g_RXBuf, T1, clientSock, from);
		g_nport = ntohs(from.sin_port);  
		//���Ӻ�������͸ÿͻ���ͨ�ŵĶ˿ں�
		if (ret>0) {
			unsigned short int operate;
			memcpy(&operate, g_RXBuf, 2);
			switch (operate) {
				case DATA:
					Type = DATA;
					recv = RecvPacket(Type);
					break;
				case ERR:
					Type = ERR;
					recv = RecvPacket(Type);
					err = true;
					Type = ReadFile_CONF;
					SendUser(Type, err);
					g_nState = IDLE;
					g_nconnect = false;
					break;
				default:
					Type = 0;
					break;
			}
			if (recv && (Type == ACK || Type == ERR || Type == ReadFile_CONF))
				break;
		}

		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);
		cout << "��ʾ���� " << g_nSendTimes + 1 << " ���ش���" << endl << endl;
	}
	// �����ʱ���ش�����>5���쳣��ֹ,�����յ������ݳ��ִ����쳣��ֹ
	// if(g_nSendTimes==MAX||Type==ERR)
	if (ret <= 0 || g_nSendTimes == MAX) {
		Type = ERR;
		err = TRUE;
		errcode = 0;
		errmsg = "���󣺳�ʱ������ʧ�ܡ�";
		g_nSendTimes = 0;

		Type = ReadFile_CONF;
		SendUser(Type, err);
		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);

		memset(g_RXBuf, '\0', 516);
		g_nState = IDLE;
		g_nconnect = false;
		return FALSE;
	}

	if (Type == ERR) {
		errcode = 0;
		errmsg = "�����ļ�����ʧ�ܡ�";
		g_nSendTimes = 0;

		Type = ReadFile_CONF;
		SendUser(Type, err);
		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);

		memset(g_RXBuf, '\0', 516);
		g_nState = IDLE;
		g_nconnect = false;
		return FALSE;
	}

	if (Type == ACK) { 
		// T2 = 1s�����ڷ�ACK�󣬼�����һ�����ݰ��ĵ���
		// cout << "�Ѿ������һ�����ݰ���ACK!" << endl;
		// setsockopt(clientSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&T2ELAPSE,sizeof(int));
		sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);
		// ret=recvd(g_RXBuf,T2,clientSock,from);
		if ((ret - 4)<512) { 
			//˵���Ѿ������һ�����ݰ���ACK,����ACK����˳�
			FileClose(g_sFileName);
			memset(g_RXBuf, '\0', 516);
			Type = ReadFile_CONF;
			err = false;
			SendUser(Type, err);
			g_nState = IDLE;
			g_nconnect = false;
		}
		else {
			g_nRecvSeqNo++;
			g_nSendTimes = 0;
			memset(g_RXBuf, '\0', 516);
			g_nconnect = true;
			g_nState = WAIT_NEXT_P;
		}
	}
	return false;
}


BOOL TFTP_WAIT_NEXT_P() {
	//����ACK��ȴ���һ���ĵ���,����ʱ�� T2 = 1000ms
	cout << "----------------------------" << endl;
	cout << "�ͻ���״̬���ȴ���һ�����ݰ����" << endl << endl;
	int send = 0;
	int ret;
	BOOL recv = false;
	int nport;  
	// Ŀ�Ķ˵Ķ˿ں�
	// ������ʱ��T2���ȴ���һ��DATA�ĵ���
	// setsockopt(clientSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&T2ELAPSE,sizeof(int));
	for (g_nSendTimes = 0; g_nSendTimes<MAX; g_nSendTimes++) {
		//#define MAX 5
		//ret=recvfrom(clientSock,g_RXBuf,516,0,(struct sockaddr*)&from,&fromLen);
		ret = recvd(g_RXBuf, T2, clientSock, from);
		nport = ntohs(from.sin_port);  // ��ʱ�յ��İ���Ŀ�Ķ˿ں�
		if (g_nport != nport) {  // ��ʱ����Ͽ�����
			errcode = 5;   // Unknown port
			errmsg = "����δ֪�˿ڡ�";
			SendPacket(ERR);
			send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);
			continue;
		}

		if (ret>0) {
			unsigned short int operate;
			memcpy(&operate, g_RXBuf, 2);
			switch (operate) {
				case DATA:
					Type = DATA;
					recv = RecvPacket(Type);
					break;
				case ERR:
					Type = ERR;
					recv = RecvPacket(Type);
					err = true;
					Type = ReadFile_CONF;
					SendUser(Type, err);
					g_nState = IDLE;

					FileClose(g_sFileName);
					g_nconnect = false;
					break;
				default:
					Type = 0;
					break;
			}
			if (recv && (Type == ACK || Type == ERR || Type == ReadFile_CONF))
				break;
		}

		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);
		cout << "��ʾ���� " << g_nSendTimes + 1 << "���ش� ACK��" << endl << endl;
	}
	// �����ʱ���ش����� >= 5 �����յ������ݳ��ִ����쳣��ֹ
	if (ret <= 0 || g_nSendTimes == MAX) {
		Type = ERR;
		err = TRUE;
		errcode = 0;
		errmsg = "���󣺳�ʱ������ʧ�ܡ�";
		g_nSendTimes = 0;

		Type = ReadFile_CONF;
		SendUser(Type, err);
		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);

		memset(g_RXBuf, '\0', 516);
		g_nState = IDLE;

		FileClose(g_sFileName);
		g_nconnect = false;
		return FALSE;
	}

	if (Type == ERR) {
		errcode = 0;
		errmsg = "�����ļ�����ʧ�ܡ�";
		g_nSendTimes = 0;

		Type = ReadFile_CONF;
		SendUser(Type, err);
		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);

		memset(g_RXBuf, '\0', 516);
		g_nState = IDLE;
		g_nconnect = false;
		return FALSE;
	}

	if (Type == ACK) {   
		// T2 = 1s�����ڷ�ACK�󣬼�����һ�����ݰ�
		// setsockopt(clientSock,SOL_SOCKET,SO_SNDTIMEO,(char *)&T2ELAPSE,sizeof(int));
		sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);
		// ret=recvd(g_RXBuf,T2,clientSock,from);
		if (ret<512) { 
			// ˵���Ѿ��ظ����һ�����ݰ���ACK
			// cout << "��ʾ���Ѿ��ظ����һ�����ݰ���ACK��" << endl;
			FileClose(g_sFileName);
			memset(g_RXBuf, '\0', 516);
			Type = ReadFile_CONF;
			err = false;
			SendUser(Type, err);
			g_nState = IDLE;
			g_nconnect = false;
		}
		else {
			g_nRecvSeqNo++;
			g_nSendTimes = 0;
			memset(g_RXBuf, '\0', 516);
			g_nconnect = true;
			g_nState = WAIT_NEXT_P;
		}
	}
	return true;
}


BOOL TFTP_WAIT_ACK() {   //����WRQ����DATA��ȴ�ACK�ĵ�� T3 = 1000ms
	cout << "----------------------------" << endl;
	cout << "�ͻ���״̬���ȴ�ACK���ݰ���" << endl << endl;
	int send = 0;
	int ret;
	BOOL recv = false;
	// ������ʱ�� T3 = 1000ms���ȴ�ACK�ĵ���
	// setsockopt(clientSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&T3ELAPSE,sizeof(int));
	for (g_nSendTimes = 0; g_nSendTimes<MAX; g_nSendTimes++) {
		// MAX = 5
		// ret=recvfrom(clientSock,g_RXBuf,516,0,(struct sockaddr*)&from,&fromLen);
		ret = recvd(g_RXBuf, T3, clientSock, from);

		if (ret>0) {
			unsigned short int operate;
			memcpy(&operate, g_RXBuf, 2);
			switch (operate) {
				case ACK:
					Type = ACK;
					recv = RecvPacket(Type);
					break;
				case ERR:
					Type = ERR;
					recv = RecvPacket(Type);
					err = true;
					Type = WriteFile_CONF;
					SendUser(Type, err);
					g_nState = IDLE;
					g_nconnect = false;
					break;
				default:
					Type = 0;
					break;
			}
			//���յ���ȷ��ACK���ܹ���ȷ��װ��һ������ʱType=DATA
			if (recv && (Type == DATA || Type == ERR || Type == WriteFile_CONF))
				break;
		}

		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);
		// cout << "�˿ںţ�" << ntohs(from.sin_port) << endl;
		if (g_nSendSeqNo == 0)
			cout << "��ʾ���� " << g_nSendTimes + 1 << " ���ش� WRQ��" << endl << endl;
		else
			cout << "��ʾ���� " << g_nSendTimes + 1 << " ���ش� DATA��" << endl << endl;
	}
	// �����ʱ���ش�����>5���쳣��ֹ,�����յ������ݳ��ִ����쳣��ֹ
	if (ret <= 0 || g_nSendTimes == MAX) {
		Type = ERR;
		err = TRUE;
		errcode = 0;
		errmsg = "���󣺳�ʱ������ʧ�ܡ�";
		g_nSendTimes = 0;

		Type = ReadFile_CONF;
		SendUser(Type, err);
		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);

		memset(g_RXBuf, '\0', 516);
		g_nState = IDLE;
		g_nconnect = false;
		return FALSE;
	}

	if (Type == ERR) {
		errcode = 0;
		errmsg = "�����ļ�����ʧ�ܡ�";
		g_nSendTimes = 0;

		Type = WriteFile_CONF;
		SendUser(Type, err);
		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);

		memset(g_RXBuf, '\0', 516);
		g_nState = IDLE;
		g_nconnect = false;
		return FALSE;
	}

	if (Type == DATA)   //���յ���ȷ��ACK���ܹ���ȷ��װ��һ������ʱType=DATA;
	{
		//T3=1s������DATA�����Ҽ���ȷ�ϰ�ACK�ĵ���
		sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);
		if (g_nLength<512) { //˵���Ѿ������һ�����ݰ�DATA
			g_nState = WAIT_LAST_ACK;
			g_nconnect = true;
		}
		else {
			g_nSendSeqNo++;
			g_nSendTimes = 0;
			memset(g_RXBuf, '\0', 516);
			g_nconnect = true;
			g_nState = WAIT_ACK;
		}
	}
	return true;
}

BOOL TFTP_WAIT_LAST_ACK() {
	cout << "�ͻ���״̬���ȴ����һ�� ACK ���ݰ���" << endl << endl;
	int send = 0;
	int ret;
	BOOL recv = false;
	//������ʱ��T3=1000ms���ȴ�Last_ACK�ĵ���
	//setsockopt(clientSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&T3ELAPSE,sizeof(int));
	for (g_nSendTimes = 0; g_nSendTimes<MAX; g_nSendTimes++)      //#define MAX 5
	{
		int fromlen = sizeof(from);
		//ret=recvfrom(clientSock,g_RXBuf,516,0,(struct sockaddr*)&from,&fromLen);

		ret = recvd(g_RXBuf, T3, clientSock, from);
		if (ret>0) {
			unsigned short int operate;
			memcpy(&operate, g_RXBuf, 2);
			switch (operate) {
				case ACK:
					Type = ACK;
					recv = RecvPacket(Type);
					break;
				case ERR:
					Type = ERR;
					recv = RecvPacket(Type);
					err = true;
					Type = ReadFile_CONF;
					SendUser(Type, err);
					g_nState = IDLE;
					g_nconnect = false;
					break;
				default:
					Type = 0;
					break;
			}
			if (recv || Type == WriteFile_CONF)
				break;
		}

		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);
		cout << "��ʾ���� " << g_nSendTimes + 1 << " ���ش����һ�� DATA ����" << endl << endl;
	}
	//�����ʱ���ش�����>5���쳣��ֹ,�����յ������ݳ��ִ����쳣��ֹ
	if ((ret <= 0 || g_nSendTimes == MAX) && (Type != WriteFile_CONF)) {
		Type = ERR;
		err = TRUE;
		errcode = 0;
		errmsg = "��ʾ����ʱ���������";
		g_nSendTimes = 0;

		Type = WriteFile_CONF;
		SendUser(Type, err);
		send = sendto(clientSock, (const char *)g_TXBuf, g_nTXBufLength, 0, (struct sockaddr*)&from, fromLen);

		FileClose(g_sFileName);  //�ر��ļ�
		memset(g_RXBuf, '\0', 516);
		memset(g_TXBuf, '\0', 516);
		g_nState = IDLE;
		g_nconnect = false;
		return FALSE;
	}

	else if (Type == WriteFile_CONF) {   //˵���Ѿ���ȷ�������һ��ACK
		FileClose(g_sFileName);
		err = FALSE;
		SendUser(Type, err);
		memset(g_TXBuf, '\0', 516);
		memset(g_RXBuf, '\0', 516);

		g_nState = IDLE;
		g_nconnect = false;
	}
	return false;
}

int recvd(char * recvBuf, int timeout, SOCKET & commSock, sockaddr_in & t_from) {
	int ret = 0;
	int t_fromLen = sizeof(t_from);
	int i = 0;
	while (i < timeout * 100) {
		memset(recvBuf, '\0', 516);  //ÿ��ʹ��ǰ��Ҫ����
		ret = recvfrom(commSock, recvBuf, 516, 0, (struct sockaddr*)&t_from, &t_fromLen);
		if (ret >= 0) {
			//��ʾ�������յ�����
			//cout<<"receive ret= "<<ret<<endl;
			return ret;
		}
		else { 
			//����SOCKET_ERROR
			Sleep(10);  //ÿ��10ms��ѯһ��
			i++;
			continue;
		}
	}
	return -1;  //��ʱ
}



