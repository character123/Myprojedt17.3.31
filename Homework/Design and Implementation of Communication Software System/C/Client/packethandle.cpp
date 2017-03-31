#include "stdafx.h"
#include "client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma pack( push, _normal_pack_ )
#pragma pack(1)

char *mode1_down = "netascii";
char *mode1_up = "netascii";
char *mode2_down = "octet";
char *mode2_up = "OCTET";

/////////////////////////////////////////////////////////////////
#pragma pack( pop, _normal_pack_ )

unsigned short int errcode;
string Mode;

char g_TXBuf[517];   //���ڷ��͵Ļ�����
char g_RXBuf[517];   //���ڽ��յĻ�����
int  g_nTXBufLength = 0;
int  g_nRXBufLength = 0;

BOOL g_bNeedSendPacket = FALSE;
BOOL g_bPacketRecv = FALSE;

void packeting(unsigned short int ntype, char *filename, string mode, char *sendbuf) {//��װRRQ��WRQ��������ntype��ȷ��
	int lengf = strlen(filename);
	int lengm = mode.size();
	if (ntype == RRQ) {
		RRQPacket pack;
		RRQPacket *p = &pack;
		p->opcode = RRQ;
		memcpy(sendbuf, &ntype, 2);
	}
	else if (ntype == WRQ) {
		WRQPacket pack;
		WRQPacket *p = &pack;
		p->opcode = WRQ;
		p->filename = filename;
		memcpy(sendbuf, &ntype, 2);
	}

	memcpy(sendbuf + 2, filename, lengf);
	sendbuf[2 + lengf] = '\0';         //end_filename='\0'
	memcpy(sendbuf + 3 + lengf, mode.data(), lengm);
	sendbuf[3 + lengf + lengm] = '\0';   //end_mode='0'
	sendbuf[3 + lengf + lengm + 1] = 0;   //����!
	g_nTXBufLength = 4 + lengf + lengm;
}

BOOL SendPacket(unsigned short int nType) {
	//cout<<"SendPacket()"<<endl;
	g_bNeedSendPacket = TRUE;
	memset(g_TXBuf, '\0', 516);    //����

	switch (nType) {
		case DATA:
		{
			//cout<<"g_nSendSeqNo="<<g_nSendSeqNo<<endl;
			memcpy(g_TXBuf, &nType, 2);
			memcpy(g_TXBuf + 2, &g_nSendSeqNo, 2);

			printf("��ʾ���������ݰ� %d��\n\n", g_nSendSeqNo);

			//int leng=strlen(g_ptr);
			printf("���ȣ�%d\n\n", g_nLength);
			memcpy(g_TXBuf + 4, g_ptr, g_nLength);
			g_TXBuf[4 + g_nLength] = '\0';
			g_nTXBufLength = g_nLength + 4;
		}
		break;

		case ACK:
		{
			memcpy(g_TXBuf, &nType, 2);
			memcpy(g_TXBuf + 2, &g_nRecvSeqNo, 2);
			g_TXBuf[4] = 0;
			g_nTXBufLength = 4;
		}
		break;

		case ERR:
		{

			memcpy(g_TXBuf, &nType, 2);
			memcpy(g_TXBuf + 2, &errcode, 2);

			cout << "����" << errmsg << endl << endl;

			int size = errmsg.size();
			memcpy(g_TXBuf + 4, errmsg.data(), size);
			g_TXBuf[4 + size] = '\0';
			g_TXBuf[4 + size + 1] = 0;
			g_nTXBufLength = size + 5;
		}
		break;
		case RRQ:
		{
			packeting(nType, g_sFileName, Mode, g_TXBuf);
		}
		break;
		case WRQ:
		{
			packeting(nType, g_sFileName, Mode, g_TXBuf);
		}
		default:
			g_bNeedSendPacket = FALSE;
			break;
	}

	return TRUE;
}


BOOL RecvPacket(unsigned short int nType)   //leng=strlen(g_RXBuf);
{
	//cout<<"RecvPacket()"<<endl;
	g_bPacketRecv = TRUE;

	//printf("Receive packet,state = %d ", g_nState);
	unsigned short int seqno;    //���ڽ��հ����
	int leng = 512;

	switch (nType) {
		case DATA:
		{
			memset(g_ptr, 0, 512);   //����
			memcpy(&seqno, g_RXBuf + 2, 2);
			if (g_nRecvSeqNo == seqno) {
				//cout<<"�յ� DATA, SeqNo="<<seqno<<endl;
				//����ǽ��յ�һ�����ݣ���Ҫ�ȴ���һ���ļ�
				if (g_nState == WAIT_FIRST_P) {
					BOOL state = FileOpen(g_sFileName, WRITE, (char *)Mode.data());
					if (!state) {
						err = true;
						errcode = 0;
						errmsg = "Cannot create the file.";
						Type = ERR;
						SendPacket(Type);
						return true;
					}

				}
				//�������ֱ������ļ�д������      
				memcpy(g_ptr, g_RXBuf + 4, leng);
				if (FileWrite(g_sFileName, leng, g_ptr)) {
					//cout<<" Write file success!"<<endl;
					Type = ACK;
					SendPacket(Type);
					return true;
				}
				else {
					FileClose(g_sFileName);
					err = true;
					errcode = 0;
					errmsg = "Load file failed.";
					Type = ERR;
					SendPacket(Type);
					g_nRXBufLength = 0;
					return true;
				}
			}

			else   //����Ų���,�ط�ACK 
			{
				memset(g_RXBuf, '\0', 516);
				Type = 0;
				return false;
			}
		}
		break;

		case ACK:
		{
			memcpy(&seqno, g_RXBuf + 2, 2);
			memset(g_RXBuf, '\0', 516);
			//cout<<"g_nSendSeqNo="<<g_nSendSeqNo<<endl;
			//cout<<"ACK seqno="<<seqno<<endl;
			if (g_nSendSeqNo == seqno) {
				if (g_nSendSeqNo == 0) {
					g_nSendSeqNo = 1;  //1
									   //��g_nSendSeqNo==0,˵���յ�ACK0���ȴ��ļ��ٶ�ȡ���ݵ�g_ptr��
									   //cout<<" Receive ACK0 !"<<endl;
					BOOL state = FileOpen(g_sFileName, READ, (char *)Mode.data());
					if (!state) {
						err = true;
						errcode = 0;
						errmsg = "Cannot open the file.";
						Type = ERR;
						SendPacket(Type);
						return true;
					}
					else {
						g_nLength = 512;
						g_nLength = FileRead(g_sFileName, g_nLength, g_ptr);
						if (g_nLength == 512) {
							//cout<<" Read file success !"<<endl;
							Type = DATA; //׼��������һ������
							SendPacket(Type);
							g_nSendSeqNo = 0; //�ָ�Ϊ0,�ȴ�����Type==DATAʱ����ż�1 
							return true;
						}
						else if ((g_nLength<512) && (g_nLength >= 0)) {
							//cout<<"���ļ������������ݳ���:"<<g_nLength<<endl;
							//���һ������
							//cout<<" Read file success ,the last packet !"<<endl;
							err = false;
							g_nSendSeqNo++;
							Type = DATA;
							SendPacket(Type);
							return true;
						}
					}
				}

				else {
					//cout<<"�յ� ACK, SeqNo="<<seqno<<endl;
					if (g_nState == WAIT_LAST_ACK) {   //�Ѿ��յ����һ��ACK���������ȷ
													   //g_nState=IDLE;
						Type = WriteFile_CONF;
						memset(g_TXBuf, '\0', 516); //���Խ���g_prt�е����һ���������
						return true;
					}
					else {
						g_nLength = 512;
						g_nLength = FileRead(g_sFileName, g_nLength, g_ptr);
						if (g_nLength == 512) {
							//cout<<" Read file success !"<<endl;
							g_nSendSeqNo++;
							Type = DATA; //׼��������һ������
							SendPacket(Type);
							g_nSendSeqNo--;
							return true;
						}
						else if ((g_nLength<512) && (g_nLength >= 0)) {
							//���һ������
							//cout<<"���ļ������������ݳ���:"<<g_nLength<<endl;
							//cout<<" Read file success ,the last packet !"<<endl;
							err = false;
							g_nSendSeqNo++;
							Type = DATA;
							SendPacket(Type);
							return true;
						}

						else {
							//���������쳣������ֹ 
							Type = WriteFile_CONF;
							FileClose(g_sFileName);
							err = true;
							errcode = 0;
							errmsg = "Load file failed.";
							Type = ERR;
							SendPacket(Type);
							return true;
						}
					}
				}
			}
			else return false;  //ACK��Ų��ԣ������κδ�����ȡ��ʱ�ش�����

		}
		break;

		case ERR:
		{
			Type = 0;
			unsigned short int errtype;
			memcpy(&errtype, g_RXBuf + 2, 2);

			cout << "��ʾ���յ�������룺" << errtype << "�����ݰ���" << endl << endl;
			errmsg = g_RXBuf + 4;
			cout << "��ʾ��������ϢΪ " << errmsg << endl << endl;
			return true;
		}
		break;

		default:
			g_bPacketRecv = FALSE;
			break;
	}
	return false;
}

