#include "stdafx.h"
#include "TFTP_server.h"
using namespace std;

void sendERR(unsigned short int errCode, string errMsg, SOCKET & commSock, sockaddr_in & t_from);
int sendDATA(unsigned short int seq, char * dataBuf, CStdioFile * pfile, SOCKET & commSock, sockaddr_in & t_from);
void reSendDATA(char * dataBuf, SOCKET & commSock, sockaddr_in & t_from);
void sendACK(unsigned short int seq, SOCKET & commSock, sockaddr_in & t_from);
void reSendACK(unsigned short int seq, SOCKET & commSock, sockaddr_in & t_from);
void plusOne(unsigned short int & seq);
int recv(char * recvBuf, int timeout, SOCKET & commSock, sockaddr_in & t_from);

// ������һ��״̬,������ص���ERROR_OVER��˵��������(���˴�����������յ������,���߳�ʱ5��)�������������
unsigned short int s_IDLE(REQPacket packet) {
	switch (packet.opCode) {
		case RRQ:
			cout << "��ʾ����ȡ�ļ��С�" << endl << endl; // ģ��ָʾԭ��
			return WAIT_R_RESP;
		case WRQ:
			cout << "��ʾ��д���ļ��С�" << endl << endl;
			return WAIT_W_RESP;
		default:
			cout << "��ʾ��״̬δ֪��" << endl << endl;
			return ERROR_OVER;
	}
}

//ǰ�ĸ������������������������Ϊ�������
unsigned short int s_WAIT_R_RESP(char*dataBuf, int & reSendTimes, unsigned short int & sendSeqNo,
	CStdioFile *pfile, REQPacket packet, SOCKET & commSock, sockaddr_in & t_from) {
	cout << "��ʾ����ȡ�ļ��С�" << endl << endl;
	int t_fromLen = sizeof(t_from);
	if (!isFileExist(packet.fileName)) //�����ļ������ڵĴ���,��ERR��
	{
		sendERR(1, "File not found", commSock, t_from);
		cout << "��ʾ������ʧ�ܣ��ļ�δ�ҵ�����WAIT_R_RESP��" << endl << endl;
		return ERROR_OVER;
	}

	//�ļ����ڣ����򲻿�
	if (!openFile(pfile, packet)) {
		sendERR(0, "File cannot be opened", commSock, t_from);
		cout << "��ʾ������ʧ�ܣ��ļ��޷��򿪡���WAIT_R_RESP��" << endl << endl;
		return ERROR_OVER;
	}

	//�ļ����ˣ���ȡ��һ������(��Ҳ���������һ������)
	sendSeqNo = 1;
	clear(dataBuf, DATA_BUF_LEN);
	int len = sendDATA(sendSeqNo, dataBuf, pfile, commSock, t_from);
	cout << "��ʾ���������ݰ� No." << sendSeqNo << " = " << len + 4 << "B" << endl << endl;
	reSendTimes = 0;
	//�ж��ǲ������һ������
	if (len < DATA_LEN) //���һ��
	{
		return WAIT_LAST_ACK;
	}
	else  //�������һ��
	{
		return WAIT_ACK;
	}
}

//ǰ������������������������Ǹ�WAIT_LAST_ACK׼�������������������Ϊ�������
unsigned short int s_WAIT_ACK(char*dataBuf, int & reSendTimes, unsigned short int & sendSeqNo,
	CStdioFile *pfile, SOCKET & commSock, sockaddr_in & t_from) {
	//��һ�η��͵�������dataBuf���棬�����ش�
	int t_fromLen = sizeof(t_from);
	unsigned short int sourcePort = ntohs(t_from.sin_port); //����Դ�˿ںţ����ں�����
	char recvBuf[RECV_BUF_LEN];
	clear(recvBuf, RECV_BUF_LEN);

	//����T3,�ȴ�ACK�ĵ���
	int timeout = T3;
	int ret = 0;
	int len = 0;
	while (1) {
		clear(recvBuf, RECV_BUF_LEN);
		ret = recv(recvBuf, timeout, commSock, t_from);
		if (ret > 0) //�յ�packet
		{
			//���ж�Դ�˿ں��Ƿ���ȷ
			if (ntohs(t_from.sin_port) != sourcePort) //Դ�˿ںŴ���err�������ǲ���ֹ����
			{
				sendERR(0, "Source port is wrong", commSock, t_from);
				continue;
			}

			//Դ�˿ں���ȷ
			switch (getOpCode(recvBuf)) {
				case ACK:
					if (decode_ACKPacket(recvBuf).seq != sendSeqNo) {
						break; //�������������ȴ�
					}
					else  //��ȷ�յ�ACK
					{
						plusOne(sendSeqNo);
						clear(dataBuf, DATA_BUF_LEN);
						len = sendDATA(sendSeqNo, dataBuf, pfile, commSock, t_from);
						cout << "��ʾ���������ݰ� No." << sendSeqNo << " = " << len + 4 << "B" << endl << endl;
						reSendTimes = 0;
						if (len < DATA_LEN) {
							return WAIT_LAST_ACK;
						}
						else {
							break;
						}
					}
				case ERR:
					cout << "��ʾ������ʧ�ܡ���WAIT_ACK  " << decode_ERRPacket(recvBuf).errMsg << "��"<< endl << endl;
					closeFile(pfile);  //ע��ر��ļ�
					return ERROR_OVER;
				default:
					break;
			}
		}
		else//��ʱ 
		{
			if (reSendTimes < MAX) //�ش�
			{
				reSendDATA(dataBuf, commSock, t_from);
				reSendTimes++;
				cout << "��ʾ���������ݰ� No." << sendSeqNo << "��" << endl << endl;
				continue;
			}
			else {
				//sendERR(0,"timeout",commSock,t_from);
				closeFile(pfile);
				cout << "��ʾ������ʧ�ܣ��޷��ط� WAIT_ACK��" << endl << endl;
				return ERROR_OVER;
			}
		}
	}
}



//���������������
unsigned short int s_WAIT_LAST_ACK(char*dataBuf, int & reSendTimes, unsigned short int & sendSeqNo,
	CStdioFile *pfile, SOCKET & commSock, sockaddr_in & t_from) {
	//��һ�η��͵�������dataBuf���棬�����ش�
	int t_fromLen = sizeof(t_from);
	unsigned short int sourcePort = ntohs(t_from.sin_port); //����Դ�˿ںţ����ں�����
	char recvBuf[RECV_BUF_LEN];
	clear(recvBuf, RECV_BUF_LEN);

	//����T3,�ȴ�ACK�ĵ���
	int timeout = T3;
	int ret = 0;
	int len = 0;
	while (1) {
		clear(recvBuf, RECV_BUF_LEN);
		ret = recv(recvBuf, timeout, commSock, t_from);
		if (ret > 0) //�յ�packet
		{
			//���ж�Դ�˿ں��Ƿ���ȷ
			if (ntohs(t_from.sin_port) != sourcePort) //Դ�˿ںŴ���err�������ǲ���ֹ����
			{
				sendERR(0, "Source port is wrong", commSock, t_from);
				continue;
			}

			//Դ�˿ں���ȷ
			switch (getOpCode(recvBuf)) {
				case ACK:
					if (decode_ACKPacket(recvBuf).seq != sendSeqNo) {
						break; //�������������ȴ�
					}
					else  //��ȷ�յ����һ����ACK
					{
						cout << "��ʾ�����سɹ���" << endl << endl;
						closeFile(pfile);
						return SUCCESS_OVER;
					}
				case ERR:
					cout << "��ʾ������ʧ�ܡ���WAIT_LAST_ACK " << decode_ERRPacket(recvBuf).errMsg << "��" << endl << endl;
					closeFile(pfile);  //ע��ر��ļ�
					return ERROR_OVER;
				default:
					break;
			}
		}
		else//��ʱ 
		{
			if (reSendTimes < MAX) //�ش�
			{
				reSendDATA(dataBuf, commSock, t_from);
				reSendTimes++;
				cout << "��ʾ������ No." << sendSeqNo << "��" << endl << endl;
				continue;
			}
			else {
				//sendERR(0,"timeout",commSock,t_from);
				closeFile(pfile);
				cout << "��ʾ������ʧ�ܣ��޷��ط� WAIT_LAST_ACK��" << endl << endl;
				return ERROR_OVER;
			}
		}
	}
}

//ǰ������������������������������
unsigned short int s_WAIT_W_RESP(int & reSendTimes, unsigned short int & recvSeqNo,
	SOCKET & commSock, sockaddr_in & t_from) {
	cout << "��ʾ��д���ļ�����WriteFile_RESP��" << endl << endl;
	recvSeqNo = 1;
	sendACK(recvSeqNo - 1, commSock, t_from);
	reSendTimes = 0;
	return WAIT_FIRST_P;
}



//ǰ����������������������ĸ����������
unsigned short int s_WAIT_FIRST_P(int & reSendTimes, unsigned short int & recvSeqNo, CStdioFile * pfile,
	REQPacket packet, SOCKET & commSock, sockaddr_in & t_from) {
	//�ȴ���һ�����ݵĵ���
	int t_fromLen = sizeof(t_from);
	unsigned short int sourcePort = ntohs(t_from.sin_port); //����Դ�˿ںţ����ں�����
	char recvBuf[RECV_BUF_LEN];
	clear(recvBuf, RECV_BUF_LEN);

	//����T2,�ȴ�data�ĵ���
	int timeout = T2;
	int ret = 0;
	int len = 0;
	while (1) {
		clear(recvBuf, RECV_BUF_LEN);
		ret = recv(recvBuf, timeout, commSock, t_from);
		cout << "��ʾ���������ݰ� No." << decode_DATAPacket(recvBuf).seq << " = " << ret << "B" << endl << endl;
		if (ret > 0) //�յ�packet
		{
			//���ж�Դ�˿ں��Ƿ���ȷ
			if (ntohs(t_from.sin_port) != sourcePort) //Դ�˿ںŴ���err�������ǲ���ֹ����
			{
				sendERR(0, "Source port is wrong", commSock, t_from);
				continue;
			}

			//Դ�˿ں���ȷ
			switch (getOpCode(recvBuf)) {
				case DATA:
					if (decode_DATAPacket(recvBuf).seq != recvSeqNo) {
						//�ش�ACK
						if (reSendTimes<MAX) {
							reSendACK(recvSeqNo - 1, commSock, t_from);
							reSendTimes++;
							cout << "��ʾ���ش� ACK��" << recvSeqNo - 1 << endl << endl;
							break;
						}
						else {
							cout << "��ʾ���ϴ�ʧ�ܣ��޷��ش� WAIT_FIRST_P��" << endl << endl;
							return ERROR_OVER;
						}
					}
					else  //��ȷ�յ���һ������
					{
						if (isFileExist(packet.fileName)) //�����ļ��Ѵ��ڵĴ���,��ERR��
						{
							sendERR(6, "File already exists", commSock, t_from);
							cout << "��ʾ���ϴ�ʧ�ܣ��ļ��Ѵ��ڡ�" << endl << endl;
							return ERROR_OVER;
						}
						//ͬ���ļ�������,���޷��������ļ�
						if (!openFile(pfile, packet)) {
							sendERR(0, "File cannot be created", commSock, t_from);
							cout << "��ʾ���ϴ�ʧ�ܣ��ļ��޷���������" << endl << endl;
							return ERROR_OVER;
						}
						//���������ļ����������ļ���д���һ������
						writeFile(pfile, decode_DATAPacket(recvBuf).data);
						sendACK(decode_DATAPacket(recvBuf).seq, commSock, t_from);
						plusOne(recvSeqNo);
						reSendTimes = 0;
						//�ж����ǲ������һ�����ݣ��ǵĻ�������ACK���˳�
						len = ret - 4; //�õ����ݲ��ֵĳ���
						if (len == DATA_LEN) //˵���������һ��
						{
							return WAIT_NEXT_P;
						}
						else {
							closeFile(pfile);
							cout << "��ʾ���ϴ��ɹ���" << endl << endl;
							return SUCCESS_OVER;
						}
					}
				case ERR:
					cout << "��ʾ���ϴ�ʧ�ܣ��� WAIT_FIRST_P ʱ��" << decode_ERRPacket(recvBuf).errMsg << endl << endl;
					return ERROR_OVER;
				default:
					break;
			}
		}
		else//��ʱ 
		{
			if (reSendTimes < MAX) //�ش�
			{
				reSendACK(recvSeqNo - 1, commSock, t_from);
				reSendTimes++;
				cout << "��ʾ���ش� ACK��" << recvSeqNo - 1 << endl << endl;
				continue;
			}
			else {
				//sendERR(0,"timeout",commSock,t_from);
				cout << "��ʾ���ϴ�ʧ�ܣ��޷��ش� WAIT_FIRST_P��" << endl << endl;
				return ERROR_OVER;
			}
		}
	}
}



//�����������
unsigned short int s_WAIT_NEXT_P(int & reSendTimes, unsigned short int & recvSeqNo,
	CStdioFile * pfile, SOCKET & commSock, sockaddr_in & t_from) {
	int t_fromLen = sizeof(t_from);
	unsigned short int sourcePort = ntohs(t_from.sin_port); //����Դ�˿ںţ����ں�����
	char recvBuf[RECV_BUF_LEN];
	clear(recvBuf, RECV_BUF_LEN);

	//����T2,�ȴ�data�ĵ���
	int timeout = T2;
	int ret = 0;
	int len = 0;
	while (1) {
		clear(recvBuf, RECV_BUF_LEN);
		ret = recv(recvBuf, timeout, commSock, t_from);
		cout << "��ʾ���������ݰ� No." << decode_DATAPacket(recvBuf).seq << " = " << ret << "B" << endl << endl;
		if (ret > 0) //�յ�����
		{
			//���ж�Դ�˿ں��Ƿ���ȷ
			if (ntohs(t_from.sin_port) != sourcePort) //Դ�˿ںŴ���err�������ǲ���ֹ����
			{
				sendERR(0, "Source port is wrong", commSock, t_from);
				continue;
			}

			//Դ�˿ں���ȷ
			switch (getOpCode(recvBuf)) {
				case DATA:
					if (decode_DATAPacket(recvBuf).seq != recvSeqNo) {
						//�ش�ACK
						if (reSendTimes<MAX) {
							reSendACK(recvSeqNo - 1, commSock, t_from);
							reSendTimes++;
							cout << "��ʾ���ط� ACK��" << recvSeqNo - 1 << endl << endl;
							break;
						}
						else {
							closeFile(pfile);
							cout << "��ʾ���ϴ�ʧ�ܣ��޷��ش�����WAIT_NEXT_P��" << endl << endl;
							return ERROR_OVER;
						}
					}
					else  //��ȷ�յ�����
					{
						//д����
						writeFile(pfile, decode_DATAPacket(recvBuf).data);
						sendACK(decode_DATAPacket(recvBuf).seq, commSock, t_from);
						plusOne(recvSeqNo);
						reSendTimes = 0;
						//�ж����ǲ������һ�����ݣ��ǵĻ�������ACK���˳�
						len = ret - 4; //�õ����ݲ��ֵĳ���
						if (len == DATA_LEN) //˵���������һ��
						{
							break;
						}
						else {
							closeFile(pfile);
							cout << "��ʾ���ϴ��ɹ���" << endl << endl;
							return SUCCESS_OVER;
						}
					}
				case ERR:
					cout << "��ʾ���ϴ�ʧ�ܣ��� WAIT_NEXT_P �У�" << decode_ERRPacket(recvBuf).errMsg << endl << endl;
					closeFile(pfile);
					return ERROR_OVER;
				default:
					break;
			}
		}
		else//��ʱ 
		{
			if (reSendTimes < MAX) //�ش�
			{
				reSendACK(recvSeqNo - 1, commSock, t_from);
				reSendTimes++;
				cout << "��ʾ���ش� ACK��" << recvSeqNo - 1 << endl << endl;
				continue;
			}
			else {
				//sendERR(0,"timeout",commSock,t_from);
				cout << "��ʾ���ϴ�ʧ�ܣ��޷��ط�����WAIT_NEXT_P��" << endl << endl;
				closeFile(pfile);
				return ERROR_OVER;
			}
		}
	}
}



void sendERR(unsigned short int errCode, string errMsg, SOCKET & commSock, sockaddr_in & t_from) {
	int t_fromLen = sizeof(t_from);
	ERRPacket ERRpacket;
	ERRpacket.opCode = ERR;
	ERRpacket.errCode = errCode;
	ERRpacket.errMsg = errMsg;
	ERRpacket.end_errMsg = '\0';
	char errBuf[ERR_BUF_LEN];
	clear(errBuf, ERR_BUF_LEN);
	code_ERRPacket(ERRpacket, errBuf);
	sendto(commSock, errBuf, ERR_BUF_LEN, 0, (struct sockaddr*)&t_from, t_fromLen);
}



//dataBuf�������������Ÿոշ����İ����Դ��ش�,����ֵ��data�ĳ���
int sendDATA(unsigned short int seq, char * dataBuf, CStdioFile * pfile, SOCKET & commSock, sockaddr_in & t_from) {
	int t_fromLen = sizeof(t_from);
	DATAPacket DATApacket;
	DATApacket.opCode = DATA;
	DATApacket.seq = seq;
	clear(DATApacket.data, DATA_LEN);  //ע��!!!
	int len = readFile(pfile, DATApacket.data);

	clear(dataBuf, DATA_BUF_LEN);
	code_DATAPacket(DATApacket, dataBuf);
	//�ж��Ƿ������һ��
	if (len == DATA_LEN)  //�������һ��
	{
		sendto(commSock, dataBuf, DATA_BUF_LEN, 0, (struct sockaddr*)&t_from, t_fromLen);
	}
	else  //�����һ������ʱ����ֱ�Ӵ�dataBuf
	{
		char *p_lastDataBuf = new char[4 + len];
		clear(p_lastDataBuf, 4 + len);
		memcpy(p_lastDataBuf, dataBuf, 4 + len);
		sendto(commSock, p_lastDataBuf, 4 + len, 0, (struct sockaddr*)&t_from, t_fromLen);
	}
	return len;
}



void reSendDATA(char * dataBuf, SOCKET & commSock, sockaddr_in & t_from) {
	int t_fromLen = sizeof(t_from);
	sendto(commSock, dataBuf, DATA_BUF_LEN, 0, (struct sockaddr*)&t_from, t_fromLen);
}

void sendACK(unsigned short int seq, SOCKET & commSock, sockaddr_in & t_from) {
	int t_fromLen = sizeof(t_from);
	ACKPacket ACKpacket;
	ACKpacket.opCode = ACK;
	ACKpacket.seq = seq;
	char ackBuf[ACK_BUF_LEN];
	clear(ackBuf, ACK_BUF_LEN);
	code_ACKPacket(ACKpacket, ackBuf);
	sendto(commSock, ackBuf, ACK_BUF_LEN, 0, (struct sockaddr*)&t_from, t_fromLen);
}



void reSendACK(unsigned short int seq, SOCKET & commSock, sockaddr_in & t_from) {
	int t_fromLen = sizeof(t_from);
	ACKPacket ACKpacket;
	ACKpacket.opCode = ACK;
	ACKpacket.seq = seq;
	char ackBuf[ACK_BUF_LEN];
	clear(ackBuf, ACK_BUF_LEN);
	code_ACKPacket(ACKpacket, ackBuf);
	sendto(commSock, ackBuf, ACK_BUF_LEN, 0, (struct sockaddr*)&t_from, t_fromLen);
}



void plusOne(unsigned short int & seq) {
	if (seq == 65535) {
		seq = 1;
	}
	else {
		seq++;
	}
}

int recv(char * recvBuf, int timeout, SOCKET & commSock, sockaddr_in & t_from) {
	int ret = 0;
	int t_fromLen = sizeof(t_from);
	int i = 0;
	while (i < timeout * 100) {
		clear(recvBuf, RECV_BUF_LEN);
		ret = recvfrom(commSock, recvBuf, RECV_BUF_LEN, 0, (struct sockaddr*)&t_from, &t_fromLen);
		if (ret >= 0)  //��ʾ�������յ�packet
		{
			return ret;
		}
		else  //����SOCKET_ERROR
		{
			Sleep(10);  //ÿ��10ms��ѯһ��
			i++;
			continue;
		}
	}
	return -1;  //��ʱ
}

