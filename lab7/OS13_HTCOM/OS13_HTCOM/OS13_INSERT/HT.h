#pragma once
#include <Windows.h>

namespace HT
{
	struct HTHANDLE    // блок управления HT
	{
		HTHANDLE();
		HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]);
		int     Capacity;               // емкость хранилища в количестве элементов 
		int     SecSnapshotInterval;    // переодичность сохранения в сек. 
		int     MaxKeyLength;           // максимальная длина ключа
		int     MaxPayloadLength;       // максимальная длина данных
		char    FileName[512];          // имя файла 
		HANDLE  File;                   // File HANDLE != 0, если файл открыт
		HANDLE  FileMapping;            // Mapping File HANDLE != 0, если mapping создан  
		LPVOID  Addr;                   // Addr != NULL, если mapview выполнен  
		char    LastErrorMessage[512];  // сообщение об последней ошибке или 0x00  
		time_t  lastsnaptime;           // дата последнего snap'a (time())  
		CRITICAL_SECTION sync;
	};

	struct Element   // элемент 
	{
		Element();
		Element(LPCVOID key, int keylength);                                             // for Get
		Element(LPCVOID key, int keylength, LPCVOID payload, int  payloadlength);    // for Insert
		Element(Element* oldelement, LPCVOID newpayload, int  newpayloadlength);         // for update
		const void*     key;                 // значение ключа 
		int             keylength;           // рахмер ключа
		const void*     payload;             // данные 
		int             payloadlength;       // размер данных
	};
};

