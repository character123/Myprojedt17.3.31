#include "stdafx.h"
#include "client.h"
#include "conio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL bStart = TRUE;
string errmsg;    //������Ϣ,ֻ���ڷ���ʱ


BOOL SendUser(unsigned short int nType, BOOL nerr) {
	switch (nType) {
		case ReadFile_CONF:
			if (!nerr) { 
				printf("��ʾ���ļ���������ɡ�\n\n");
			}
			else {
				printf("��ʾ���ļ�����ʧ�ܡ�\n���ִ���");
				cout << errmsg << endl << endl;
			}
			memset(g_sFileName, '\0', 50);
			bStart = FALSE;
			break;
		case WriteFile_CONF:
			//printf("WRITE FILE SEND CONFIRM %s \n", g_sFileName);
			if (!nerr) {
				printf("��ʾ���ļ��ϴ�����ɡ�\n\n");
			}
			else {
				printf("��ʾ���ļ��ϴ�ʧ�ܡ�\n���ִ���");
				cout << errmsg << endl << endl;
			}
			memset(g_sFileName, '\0', 50);
			bStart = FALSE;
			break;
	}
	return TRUE;
}

BOOL GetFromUser() {
	char ch;
	if (bStart) {
		cout << "ѡ�\n1. �����ļ���\n2. �ϴ��ļ���\n";
		cout << "n. �˳�\n\n";
		cout << "������ѡ�";
		bStart = FALSE;
	}

	cin >> ch;
	if (ch == 'n' || ch == 'N') {
		cout << "��л����ʹ�á�" << endl << endl;
		exit(0);
	}
	else if (ch == '1') {
		g_nEvent = ReadFile_REQ;
	}
	else if (ch == '2') {
		g_nEvent = WriteFile_REQ;
	}
	cout << "\n�����ļ�����";
	cin >> g_sFileName;
	cout << endl;
	cout << "��ʾ���ļ�Ϊ " << g_sFileName << " ��" << endl << endl;

	return TRUE;
}