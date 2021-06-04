#include "HT.h"
#include <Windows.h>
#include <iostream>
#include <time.h>

namespace HT {

	HT::HTHANDLE::HTHANDLE()
	{

	}

	HT::HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512])
	{
		this->Capacity = Capacity;
		this->SecSnapshotInterval = SecSnapshotInterval;
		this->MaxKeyLength = MaxKeyLength;
		this->MaxPayloadLength = MaxPayloadLength;
		this->File = NULL;
		this->FileMapping = NULL;
		this->Addr = NULL;
		memset(this->LastErrorMessage, 0, sizeof(this->LastErrorMessage));
		this->lastsnaptime = time(NULL);
		memcpy(this->FileName, FileName, 512);
	}

	HT::Element::Element(LPCVOID key, int keylength)
	{
		this->key = key;
		this->keylength = keylength;
		this->payload = NULL;
		this->payloadlength = NULL;
	}

	HT::Element::Element(LPCVOID key, int keylength, LPCVOID payload, int payloadlength	)
	{
		this->key = key;
		this->keylength = keylength;
		this->payload = payload;
		this->payloadlength = payloadlength;
	}

	HT::Element::Element(Element* oldelement, LPCVOID newpayload, int newpayloadlength)
	{
		this->key = oldelement->key;
		this->keylength = oldelement->keylength;
		this->payload = newpayload;
		this->payloadlength = newpayloadlength;
	}

	HTHANDLE* Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512])
	{
		try {
			HTHANDLE*	NewHT = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);
			
			HANDLE hf = CreateFileA(
				FileName,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (hf == INVALID_HANDLE_VALUE)
				throw "create file failed";

			DWORD numberOfMetaBytesWritten;
			WriteFile(
				hf,
				NewHT,
				sizeof(HT::HTHANDLE),
				&numberOfMetaBytesWritten,
				NULL
			);

			NewHT->File = hf;
			CloseHandle(hf);
			return NewHT;
		}
		catch (const char* em) {
			std::cout << "Error: " << em << " \n";
			
		}
	}

	HTHANDLE* Open(const char FileName[512])
	{
		try {
			HANDLE hf = CreateFileA(
				FileName,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (hf == INVALID_HANDLE_VALUE)
				throw "open file failed";
			HT::HTHANDLE GetHT;
			DWORD numberOfBytesRead;
			ReadFile(
				hf,
				&GetHT,
				sizeof(HT::HTHANDLE),
				&numberOfBytesRead,
				NULL
			);
			HT::HTHANDLE* NewHT = new HT::HTHANDLE(GetHT.Capacity, GetHT.SecSnapshotInterval, GetHT.MaxKeyLength, GetHT.MaxPayloadLength, FileName);
			NewHT->File = hf;
			HANDLE hm = CreateFileMapping(
				hf, NULL,
				PAGE_READWRITE,
				NULL, 
				sizeof(NewHT)+ NewHT->Capacity*(NewHT->MaxKeyLength + NewHT->MaxPayloadLength)
				, NULL);
			if (!hm)
				throw "create File mapping failed";

			LPVOID lp = MapViewOfFile(
				hm,
				FILE_MAP_ALL_ACCESS,
				NULL, 0,
				NewHT->Capacity*(NewHT->MaxKeyLength + NewHT->MaxPayloadLength));
			if (!lp)
				throw "mapping view failed";

			NewHT->Addr = lp;
			NewHT->File = hf;
			NewHT->FileMapping = hm;
			InitializeCriticalSection(&(NewHT->sync));

			HANDLE SnapThread = CreateThread(NULL, NULL, SnapTimer, (LPVOID)NewHT, NULL, NULL);

			return NewHT;

		}
		catch (const char* em) {
			std::cout << "Error: " << em << " \n";
		}
	}

	BOOL Snap(const HTHANDLE* hthandle)
	{
		try {
			EnterCriticalSection((LPCRITICAL_SECTION) & (hthandle->sync));
			FlushViewOfFile(hthandle->Addr, hthandle->Capacity);
			FlushFileBuffers(hthandle->File);
			LeaveCriticalSection((LPCRITICAL_SECTION) & (hthandle->sync));
		}
		catch (const char* em) {
			std::cout << "Error: " << em << " \n";
		}

		return 1;
	}

	DWORD WINAPI SnapTimer(LPVOID hthandle)
	{
			int snapTime = ((HT::HTHANDLE*)hthandle)->SecSnapshotInterval;

			while(true)
			{
				Sleep(snapTime);
				Snap(((HT::HTHANDLE*)hthandle));
			}
	}

	BOOL Close(const HTHANDLE* hthandle)
	{
		Snap(hthandle);
		UnmapViewOfFile(hthandle->Addr);
		CloseHandle(hthandle->FileMapping);
		CloseHandle(hthandle->File);
		return 0;
	}

	bool IsMemEmpty(LPCVOID mem, int len)
	{
		LPVOID zeroBuf = malloc(len);
		memset(zeroBuf, 0, len);

		bool res = (0 == memcmp(zeroBuf, mem, len));

		free(zeroBuf);

		return res;
	}

	int GenerateHash(LPCVOID bytes, int numOfBytes)
	{
		LPBYTE pByte = (LPBYTE)bytes;
		int res = 0;

		for (int i = 0; i < numOfBytes; i++)
		{
			res ^= *(pByte + i) << (8 * (i % sizeof(int)));
		}

		return (int)abs(((float)res / 1.61803));
	}

	BOOL Insert(const HTHANDLE* hthandle, const Element*  element)
	{
		try {
			int firstEmptyRecordInd = -1;
			int maxKeyLength = hthandle->MaxKeyLength;

			int hash = GenerateHash(element->key, element->keylength) % hthandle->Capacity;

			for (int ii = hash; ii < hthandle->Capacity + hash; ii++)
			{ 
				int i = ii % hthandle->Capacity;

				LPCVOID nKey = (LPBYTE)(hthandle->Addr) + i * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength);

				if (IsMemEmpty(nKey, maxKeyLength))
					firstEmptyRecordInd = firstEmptyRecordInd == -1 ? i : firstEmptyRecordInd;

				LPVOID maxLenKey = malloc(maxKeyLength);
				memset(maxLenKey, 0, maxKeyLength);
				memcpy(maxLenKey, element->key, element->keylength);

				if (!memcmp(nKey, maxLenKey, maxKeyLength))
				{
					free(maxLenKey);

					memcpy((void*)hthandle->LastErrorMessage, "Key exists", 512);
					throw hthandle->LastErrorMessage;
					return FALSE;
				}
				free(maxLenKey);
			}

			if (firstEmptyRecordInd == -1)
			{
				memcpy((void*)hthandle->LastErrorMessage, "Full", 512);
				throw hthandle->LastErrorMessage;
				return FALSE;
			}

			LPVOID nKey = (LPBYTE)(hthandle->Addr) + firstEmptyRecordInd * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength);
			LPVOID nPayload = (LPBYTE)(hthandle->Addr) + firstEmptyRecordInd * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength) + hthandle->MaxKeyLength;

			memcpy(nKey, element->key, element->keylength);
			memcpy(nPayload, element->payload, element->payloadlength);
		}
		catch (const char* em) {
			std::cout << "Error: " << em << " \n";
		}
		return TRUE;
	}

	BOOL Delete(const HTHANDLE* hthandle, const Element*  element)
	{
		int maxKeyLength = hthandle->MaxKeyLength;

		int hash = GenerateHash(element->key, element->keylength) % hthandle->Capacity;

		for (int ii = hash; ii < hthandle->Capacity + hash; ii++)
		{
			int i = ii % hthandle->Capacity;

			LPVOID maxLenKey = malloc(maxKeyLength);
			memset(maxLenKey, 0, maxKeyLength);
			memcpy(maxLenKey, element->key, element->keylength);

			LPVOID nKey =(LPBYTE)(hthandle->Addr) + i * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength);

			if (0 == memcmp(maxLenKey, nKey, maxKeyLength))
			{
				memset(nKey, 0, maxKeyLength + hthandle->MaxPayloadLength);

				free(maxLenKey);
				return TRUE;
			}
			free(maxLenKey);
		}

		memcpy((void*)hthandle->LastErrorMessage, "There is no such element to delete", 512);
		return FALSE;	
	}

	Element* Get(const HTHANDLE* hthandle, const Element*  element) 
	{
		try {
			int maxKeyLength = hthandle->MaxKeyLength;
			int maxPayloadLength = hthandle->MaxPayloadLength;

			int hash = GenerateHash(element->key, element->keylength) % hthandle->Capacity;

			for (int ii = hash; ii < hthandle->Capacity + hash; ii++)
			{
				int i = ii % hthandle->Capacity;

				LPVOID maxLenKey = malloc(maxKeyLength);
				memset(maxLenKey, 0, maxKeyLength);
				memcpy(maxLenKey, element->key, element->keylength);

				LPVOID nKey = (LPBYTE)(hthandle->Addr) + i * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength);

				if (0 == memcmp(maxLenKey, nKey, maxKeyLength))
				{
					LPVOID newElKey = malloc(element->keylength);
					memcpy(newElKey, nKey, element->keylength);

					LPVOID newElPayload = malloc(maxPayloadLength);
					memcpy(newElPayload, (LPVOID)((LPBYTE)(hthandle->Addr) + i * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength) + hthandle->MaxKeyLength), maxPayloadLength);

					HT::Element* el = new HT::Element(
						newElKey,
						element->keylength,
						newElPayload,
						hthandle->MaxPayloadLength
					);

					free(maxLenKey);
					return el;
				}
				free(maxLenKey);
			}
			memcpy((void*)hthandle->LastErrorMessage, "No such key", 512);
			throw hthandle->LastErrorMessage;
		}
		catch (const char* em) {
			std::cout << "Error: " << em << " \n";
		}
		return NULL;
	}

	BOOL Update (const HTHANDLE* hthandle, const Element*  oldelement, const void* newpayload, int newpayloadlength)
	{
		int maxKeyLength = hthandle->MaxKeyLength;
		int maxPayloadLength = hthandle->MaxPayloadLength;

		int hash = GenerateHash(oldelement->key, oldelement->keylength) % hthandle->Capacity;

		for (int ii = hash; ii < hthandle->Capacity + hash; ii++)
		{
			int i = ii % hthandle->Capacity;

			LPVOID maxLenKey = malloc(maxKeyLength);
			memset(maxLenKey, 0, maxKeyLength);
			memcpy(maxLenKey, oldelement->key, oldelement->keylength);

			LPVOID nKey = (LPBYTE)(hthandle->Addr) + i * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength);

			if (0 == memcmp(nKey, maxLenKey, maxKeyLength))
			{
				LPVOID oldPayload = (LPBYTE)(hthandle->Addr) + i * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength) + hthandle->MaxKeyLength;
				memset(oldPayload, 0, maxPayloadLength);
				memcpy(oldPayload, newpayload, newpayloadlength);

				free(maxLenKey);

				return TRUE;
			}
			free(maxLenKey);
		}

		memcpy((void*)hthandle->LastErrorMessage, "No such element to update", 512);
		return FALSE;
	}

	char* GetLastError (HTHANDLE* ht)
	{
		return ht->LastErrorMessage;
	}

	void print (const Element*  element)
	{
		try {
			if (element) {
				std::cout << "Key = " << *(int*)element->key
					<< ", KeyLength = " << element->keylength
					<< ", Payload = " << *(int*)element->payload
					<< ", PayloadLength = " << element->payloadlength << "\n";

				OutputDebugStringA("Hello World");
			}
			else {
				throw "Element is not exist in HT";
			}
		}
		catch (const char* em) {
			std::cout << "Error: " << em << " \n";
		}
	}
}
