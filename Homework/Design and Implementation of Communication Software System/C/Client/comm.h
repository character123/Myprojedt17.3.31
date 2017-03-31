#include <iostream>
#include <string>
using namespace std;

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

class CInitSock	   //��װCInitSock��	
{
public:
	CInitSock(BYTE minorVer = 2, BYTE majorVer = 2)
	{
		// ��ʼ��WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer, majorVer);  //ָ��Ҫ���ص�Winsock��İ汾 
		if(::WSAStartup(sockVersion, &wsaData) != 0)    //WSAStartup��������Winsock��
		{
			exit(0);
		}
	}
	~CInitSock()
	{	
		::WSACleanup();	  //�ͷ�Winsock��
	}
};