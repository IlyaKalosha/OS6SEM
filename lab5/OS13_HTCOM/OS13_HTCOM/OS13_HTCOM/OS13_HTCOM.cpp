#include "pch.h"
#include "OS13_HTCOM.h"
#include "HT.h"
#include "SEQLOG.h"
#include <iostream>
#include <ctime>


extern ULONG g_Components;

OS13_HTCOM::OS13_HTCOM() :m_Ref(1)
{
	SEQ;
	InterlockedIncrement((LONG*)&g_Components);
	LOG("Adder g_Components = ", g_Components);
}

OS13_HTCOM::~OS13_HTCOM()
{
	SEQ;
	InterlockedIncrement((LONG*)&g_Components);
	LOG("Adder g_Components = ", g_Components);
}

DWORD WINAPI SnapTimer(LPVOID hthandle)
{
	int snapTime = ((HT::HTHANDLE*)hthandle)->SecSnapshotInterval;
	std::ofstream out("C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab5\\out.txt");
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());
	while (true)
	{
		Sleep(snapTime);
		try {
			EnterCriticalSection((LPCRITICAL_SECTION) & (((HT::HTHANDLE*)hthandle)->sync));
			FlushViewOfFile(((HT::HTHANDLE*)hthandle)->Addr, ((HT::HTHANDLE*)hthandle)->Capacity);
			FlushFileBuffers(((HT::HTHANDLE*)hthandle)->File);
			LeaveCriticalSection((LPCRITICAL_SECTION) & (((HT::HTHANDLE*)hthandle)->sync));
		}
		catch (const char* em) {
			std::cout << "Error: " << em << " \n";
			return 0;
		}
		std::cout << "Snap\n";
	}
}

HRESULT STDMETHODCALLTYPE OS13_HTCOM::QueryInterface(REFIID riid, void ** ppvObject)
{
	SEQ;
	HRESULT rc = S_OK;
	*ppvObject = NULL;
	if (riid == IID_IUnknown)	*ppvObject = (IHTCreate*)this;
	else if (riid == IID_IHTCreate)	*ppvObject = (IHTCreate*)this;
	else if (riid == IID_IHTStart)	*ppvObject = (IHTStart*)this;
	else if (riid == IID_IHTStop)	*ppvObject = (IHTStop*)this;
	else if (riid == IID_IHTInsert)	*ppvObject = (IHTInsert*)this;
	else if (riid == IID_IHTUpdate)	*ppvObject = (IHTUpdate*)this;
	else if (riid == IID_IHTDelete)	*ppvObject = (IHTDelete*)this;
	else rc = E_NOINTERFACE;

	if (rc == S_OK) this->AddRef();
	LOG("QueryInterface rc = ", rc);
	return rc;
}

ULONG STDMETHODCALLTYPE OS13_HTCOM::AddRef(void)
{
	SEQ;
	InterlockedIncrement((LONG*)&(this->m_Ref));
	LOG("AddRef m_Ref = ", this->m_Ref);
	return this->m_Ref;
}

ULONG STDMETHODCALLTYPE OS13_HTCOM::Release(void)
{
	SEQ;
	ULONG rc = this->m_Ref;
	if ((rc = InterlockedDecrement((LONG*)&(this->m_Ref))) == 0) delete this;
	LOG("Release rc = ", rc);
	return rc;
}

HRESULT  STDMETHODCALLTYPE OS13_HTCOM::HTCreate(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512], HT::HTHANDLE*& htHandle)
{
	try {
		HT::HTHANDLE*	NewHT = new HT::HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);

		HANDLE hf = CreateFileA(
			FileName,
			GENERIC_READ | GENERIC_WRITE,
			NULL,
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
		htHandle = NewHT;
	}
	catch (const char* em) {
		std::cout << "Error: " << em << " \n";

	}
}

HRESULT  STDMETHODCALLTYPE OS13_HTCOM::HTOpen(const char FileName[512], HT::HTHANDLE*& htHandle)
{
	try {
		HANDLE hf = CreateFileA(
			FileName,
			GENERIC_READ | GENERIC_WRITE,
			NULL,
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
			sizeof(NewHT) + NewHT->Capacity*(NewHT->MaxKeyLength + NewHT->MaxPayloadLength)
			, L"FileMappingName");
		if (!hm)
			throw "create File mapping failed";



		NewHT->File = hf;
		NewHT->FileMapping = hm;
		InitializeCriticalSection(&(NewHT->sync));

		HANDLE SnapThread = CreateThread(NULL, NULL, SnapTimer, (LPVOID)NewHT, NULL, NULL);
		if (!SnapThread)
			throw "SnapTimer failed";
		htHandle = NewHT;

	}
	catch (const char* em) {
		std::cout << "Error: " << em << " \n";
		htHandle = NULL;
	}
}

HRESULT STDMETHODCALLTYPE OS13_HTCOM::HTSnap(const HT::HTHANDLE * hthandle, BOOL & result)
{
	try {
		EnterCriticalSection((LPCRITICAL_SECTION) & (hthandle->sync));
		FlushViewOfFile(hthandle->Addr, hthandle->Capacity);
		FlushFileBuffers(hthandle->File);
		LeaveCriticalSection((LPCRITICAL_SECTION) & (hthandle->sync));
	}
	catch (const char* em) {
		std::cout << "Error: " << em << " \n";
		result = 0;
		return 0;
	}
	result = 1;
	return 1;
}

HRESULT STDMETHODCALLTYPE OS13_HTCOM::HTClose(const HT::HTHANDLE * hthandle, BOOL & result)
{
	try {
		UnmapViewOfFile(hthandle->Addr);
		CloseHandle(hthandle->FileMapping);
		CloseHandle(hthandle->File);
	}
	catch (const char* em) {
		std::cout << "Error: " << em << " \n";
		result = 0;
		return 0;
	}
	result = 1;
	return 1;
}

HRESULT STDMETHODCALLTYPE OS13_HTCOM::HTInsert(const HT::HTHANDLE * hthandle, const HT::Element * element, BOOL & result)
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
				result = FALSE;
			}
			free(maxLenKey);
		}

		if (firstEmptyRecordInd == -1)
		{
			memcpy((void*)hthandle->LastErrorMessage, "Full", 512);
			throw hthandle->LastErrorMessage;
			result = FALSE;
		}

		LPVOID nKey = (LPBYTE)(hthandle->Addr) + firstEmptyRecordInd * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength);
		LPVOID nPayload = (LPBYTE)(hthandle->Addr) + firstEmptyRecordInd * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength) + hthandle->MaxKeyLength;

		memcpy(nKey, element->key, element->keylength);
		memcpy(nPayload, element->payload, element->payloadlength);
	}
	catch (const char* em) {
		std::cout << "Error: " << em << " \n";
	}
	result = TRUE;
}

HRESULT STDMETHODCALLTYPE OS13_HTCOM::HTOpenMapView(const char MappingName[512], LPVOID & lp)
{
	HANDLE FileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, true, MappingName);
	LPVOID clp = MapViewOfFile(
		FileMapping,
		FILE_MAP_ALL_ACCESS,
		NULL, 0,
		0);
	if (!clp) {

		throw "mapping view failed";
		return NULL;
		lp = NULL;
	}
	lp = clp;
}

HRESULT STDMETHODCALLTYPE OS13_HTCOM::ElemCreate(LPCVOID key, int keylength, LPCVOID payload, int payloadlength, HT::Element*& elem)
{
	try {
		elem = new HT::Element(key, keylength, payload, payloadlength);
		return 1;
	}
	catch (const char* em) {
		std::cout << "Error: " << em << " \n";
		elem = NULL;
		return 0;
	}
}

HRESULT STDMETHODCALLTYPE OS13_HTCOM::HTUpdate(const HT::HTHANDLE * hthandle, const HT::Element * oldelement, const void * newpayload, int newpayloadlength, BOOL & result)
{
	try {
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

				result = TRUE;
				return TRUE;
			}
			free(maxLenKey);
		}

		memcpy((void*)hthandle->LastErrorMessage, "No such element to update", 512);
		throw hthandle->LastErrorMessage;
	}
	catch (const char* em) {
		std::cout << "Error: " << em << " \n";
		result = FALSE;
		return FALSE;
	}
	result = FALSE;
	return FALSE;
}

HRESULT STDMETHODCALLTYPE OS13_HTCOM::HTDelete(const HT::HTHANDLE * hthandle, const HT::Element * element, BOOL & result)
{
	try {
		int maxKeyLength = hthandle->MaxKeyLength;

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
				memset(nKey, 0, maxKeyLength + hthandle->MaxPayloadLength);

				free(maxLenKey);
				result = TRUE;
				return TRUE;
			}
			free(maxLenKey);
		}

		memcpy((void*)hthandle->LastErrorMessage, "There is no such element to delete", 512);
		throw hthandle->LastErrorMessage;

	}
	catch (const char* em) {
		std::cout << "Error: " << em << " \n";
		result = FALSE;
		return FALSE;
	}
	
}

HRESULT STDMETHODCALLTYPE OS13_HTCOM::HTGet(const HT::HTHANDLE * hthandle, const HT::Element * element, HT::Element*& getElement)
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
				getElement = el;
				return NULL;
			}
			free(maxLenKey);
		}
		memcpy((void*)hthandle->LastErrorMessage, "No such key", 512);
		throw hthandle->LastErrorMessage;
	}
	catch (const char* em) {
		std::cout << "Error: " << em << " \n";
		getElement = NULL;
		return NULL;
	}
	getElement = NULL;
	return NULL;
}

char* OS13_HTCOM::GetLastError(HT::HTHANDLE* ht)
{
	return ht->LastErrorMessage;
}

void OS13_HTCOM::print(const HT::Element*  element)
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

bool OS13_HTCOM::IsMemEmpty(LPCVOID mem, int len)
{
	LPVOID zeroBuf = malloc(len);
	memset(zeroBuf, 0, len);

	bool res = (0 == memcmp(zeroBuf, mem, len));

	free(zeroBuf);

	return res;
}

int OS13_HTCOM::GenerateHash(LPCVOID bytes, int numOfBytes)
{
	LPBYTE pByte = (LPBYTE)bytes;
	int res = 0;

	for (int i = 0; i < numOfBytes; i++)
	{
		res ^= *(pByte + i) << (8 * (i % sizeof(int)));
	}

	return (int)abs(((float)res / 1.61803));
}

