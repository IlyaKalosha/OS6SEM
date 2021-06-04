#pragma once
#include <Windows.h>

namespace HT
{
	struct HTHANDLE    // ���� ���������� HT
	{
		HTHANDLE();
		HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]);
		int     Capacity;               // ������� ��������� � ���������� ��������� 
		int     SecSnapshotInterval;    // ������������� ���������� � ���. 
		int     MaxKeyLength;           // ������������ ����� �����
		int     MaxPayloadLength;       // ������������ ����� ������
		char    FileName[512];          // ��� ����� 
		HANDLE  File;                   // File HANDLE != 0, ���� ���� ������
		HANDLE  FileMapping;            // Mapping File HANDLE != 0, ���� mapping ������  
		LPVOID  Addr;                   // Addr != NULL, ���� mapview ��������  
		char    LastErrorMessage[512];  // ��������� �� ��������� ������ ��� 0x00  
		time_t  lastsnaptime;           // ���� ���������� snap'a (time())  
		CRITICAL_SECTION sync;
	};

	struct Element   // ������� 
	{
		Element();
		Element(LPCVOID key, int keylength);                                             // for Get
		Element(LPCVOID key, int keylength, LPCVOID payload, int  payloadlength);    // for Insert
		Element(Element* oldelement, LPCVOID newpayload, int  newpayloadlength);         // for update
		const void*     key;                 // �������� ����� 
		int             keylength;           // ������ �����
		const void*     payload;             // ������ 
		int             payloadlength;       // ������ ������
	};
};

