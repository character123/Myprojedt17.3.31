#include "stdafx.h"
#include "TFTP_server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWinApp theApp;
using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
	int nRetCode = 0;
	// ��ʼ�� MFC�������������ʾ
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		cerr << _T("����MFC ��ʼ��ʧ�ܡ�") << endl << endl;
		nRetCode = 1;
	}
	else {
		startUp();
	}
	return nRetCode;
}

void clear(char*a, int size) {
	int i = 0;
	while (i < size) {
		a[i] = '\0';
		i++;
	}
}
