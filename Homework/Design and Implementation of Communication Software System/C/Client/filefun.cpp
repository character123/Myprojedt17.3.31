#include "stdafx.h"
#include "client.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStdioFile file;

BOOL isFileExist(char *filename) {
	//�ж��ļ��Ƿ����
	HANDLE  hFindFile;
	WIN32_FIND_DATA  FileData;

	hFindFile = FindFirstFile("resource\\*", &FileData);   //����"."����ļ�

	if (hFindFile == INVALID_HANDLE_VALUE) {
		return false;
	}
	else {
		//�����ӡ�����ļ�����ǰ���У���һ����".",�ڶ�����".."��
		//"."��ʾ��ǰĿ¼,".."��ʾ�ϲ�Ŀ¼.DOS���ļ�ϵͳ�а��������������ļ�,������СΪ0
		FindNextFile(hFindFile, &FileData);  //����Ϊ�˴���".."�Ǹ��ļ�
		char br[2] = { '\n','\0' };
		while (FindNextFile(hFindFile, &FileData)) {
			if (strstr(FileData.cFileName, filename) == NULL)
				continue;
			else {
				FindClose(hFindFile); //˵���ļ�����
				return TRUE;
			}
		}
		FindClose(hFindFile);
		return FALSE;
	}
}

BOOL FileOpen(char * sFileName, unsigned short int nType, char * mode) {
	int flag;
	string path = "resource\\";
	string filePath = path + sFileName;

	if (nType == READ) {
		// ���ļ����ϴ�
		if (strcmp(mode, mode1_down) == 0 || strcmp(mode, mode1_up) == 0) {
			flag = file.Open(filePath.data(), CStdioFile::modeRead | CStdioFile::typeText);
			if (flag > 0)
				return true;
			else
				return false;
		}
		else if (strcmp(mode, mode2_down) == 0 || strcmp(mode, mode2_up) == 0) {
			flag = file.Open(filePath.data(), CStdioFile::modeRead | CStdioFile::typeBinary);
			if (flag > 0)
				return true;
			else
				return false;
		}
	}
	else {
		//д�ļ�,����
		if (strcmp(mode, mode1_down) == 0 || strcmp(mode, mode1_up) == 0) {
			// �ı�
			flag = file.Open(filePath.data(), CStdioFile::modeCreate | CStdioFile::modeWrite |
				CStdioFile::typeText);
			if (flag > 0)
				return true;
			else
				return false;
		}
		else if (strcmp(mode, mode2_down) == 0 || strcmp(mode, mode2_up) == 0) {
			flag = file.Open(filePath.data(), CStdioFile::modeCreate | CStdioFile::modeWrite |
				CStdioFile::typeBinary);
			if (flag > 0)
				return true;
			else
				return false;
		}
		else return false;
	}
	return false;
}

BOOL FileClose(char * sFileName) {
	file.Close();
	return true;
}

//ptr�������������СΪ512�ֽ�,����ֵ��ʵ�ʶ������ַ���
int FileRead(char * sFileName, int & nLength, char * ptr) {
	int len = 0;
	memset(ptr, 0, 512);
	len = file.Read(ptr, nLength);
	return len;
}

//prt���������
BOOL FileWrite(char * sFileName, int nLength, char * ptr) {
	file.Write(ptr, nLength);
	return true;
}
