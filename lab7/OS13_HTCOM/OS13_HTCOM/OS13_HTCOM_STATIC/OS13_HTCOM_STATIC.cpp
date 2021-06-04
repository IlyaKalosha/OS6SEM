// OS13_HTCOM_STATIC.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"
#include "OS13_HTCOM_STATIC.h"
#include <objbase.h>
#include <iostream>
#include <random>
#include <ctime>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>

IHTCreate* pIHTCreate = nullptr;
IHTStart* pIHTStart = nullptr;
IHTStop* pIHTStop = nullptr;
IHTInsert* pIHTInsert = nullptr;
IHTUpdate *pIHTUpdate = nullptr;
IHTDelete* pIHTDelete = nullptr;

OS13HANDLE HTCOM_STATIC::Init()
{
	IUnknown* pIUnknown = NULL;
	CoInitialize(NULL);                        // инициализация библиотеки OLE32
	HRESULT hr0 = CoCreateInstance(CLSID_CA, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pIUnknown);
	if (SUCCEEDED(hr0))
	{
		return pIUnknown;
	}
	else {
		throw (int)hr0;
		return NULL;
	}
}

void HTCOM_STATIC::Dispose(OS13HANDLE h)
{
	((IUnknown*)h)->Release();
	CoFreeUnusedLibraries();
}

HT::HTHANDLE* HTCOM_STATIC::Create(OS13HANDLE h, int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char HTUsersGroup[512], const char FileName[512])
{
	try {
		HT::HTHANDLE* result;
		HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHTCreate, (void**)&pIHTCreate);
		if (SUCCEEDED(hr0))
		{
			HRESULT hr1 = pIHTCreate->HTCreate(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, HTUsersGroup, FileName, result);
			if (SUCCEEDED(hr1))
			{
				pIHTCreate->Release();
				return result;
			}
			else
			{
				pIHTCreate->Release();
				throw (int)hr1;
				return NULL;
			}
		}
		else
		{

			throw (int)hr0;
			return NULL;
		}
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
		return 0;
	}
}

HT::HTHANDLE* HTCOM_STATIC::Start(OS13HANDLE h, const char FileName[512])
{
	try{
	HT::HTHANDLE* result;
	HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHTStart, (void**)&pIHTStart);
	if (SUCCEEDED(hr0))
	{
		HRESULT hr1 = pIHTStart->HTOpen(FileName, result);
		if (SUCCEEDED(hr1))
		{
			pIHTStart->Release();
			return result;
		}
		else
		{
			pIHTStart->Release();
			throw (int)hr1;
			return NULL;
		}
	}
	else
	{

		throw (int)hr0;
		return NULL;
	}
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
		return 0;
	}
}

HT::HTHANDLE* HTCOM_STATIC::Start2(OS13HANDLE h, const char HTUser[512], const char HTPassword[512], const char FileName[512])
{
	try {
		HT::HTHANDLE* result;
		HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHTStart, (void**)&pIHTStart);
		if (SUCCEEDED(hr0))
		{
			HRESULT hr1 = pIHTStart->HTOpen(HTUser, HTPassword, FileName, result);
			if (SUCCEEDED(hr1))
			{
				pIHTStart->Release();
				return result;
			}
			else
			{
				pIHTStart->Release();
				throw (int)hr1;
				return NULL;
			}
		}
		else
		{

			throw (int)hr0;
			return NULL;
		}
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
		return 0;
	}
}

void HTCOM_STATIC::Stop(OS13HANDLE h, const wchar_t* Process1Name, const wchar_t* Process2Name, const wchar_t* Process3Name, const wchar_t* Process4Name)
{
	try {
		HTCOM_STATIC::killProcessByName(Process1Name);
		HTCOM_STATIC::killProcessByName(Process2Name);
		HTCOM_STATIC::killProcessByName(Process3Name);
		HTCOM_STATIC::killProcessByName(Process4Name);
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}

void HTCOM_STATIC::Insert(OS13HANDLE h, const char MappingName[512])
{
	try {
		HT::HTHANDLE* StorageInstance;
		LPVOID MappinView;
		HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHTInsert, (void**)&pIHTInsert);
		if (SUCCEEDED(hr0))
		{
			HRESULT hr1 = pIHTInsert->HTOpenMapView(MappingName, MappinView);
			StorageInstance = reinterpret_cast<HT::HTHANDLE*>(MappinView);
			if (SUCCEEDED(hr1))
			{
				for (;;)
				{
					StorageInstance->Addr = MappinView;
					std::mt19937 engine; // mt19937 как один из вариантов
					engine.seed(std::time(nullptr) ^ _getpid());
					int key = ((engine()) % 50) + 1;

					int value = 0;

					HT::Element* elem;
					pIHTInsert->ElemCreate(&key, sizeof(key), &value, sizeof(int), elem);
					BOOL res;
					std::cout << "HT_INSERT: key - " << key << ", value - " << value << "\n";
					pIHTInsert->HTInsert(StorageInstance, elem, res);
					Sleep(1000);
				}
				pIHTInsert->Release();
			}
			else
			{
				pIHTInsert->Release();
				throw (int)hr1;
			}
		}
		else
		{

			throw (int)hr0;
		}
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}

void HTCOM_STATIC::Update(OS13HANDLE h, const char MappingName[512])
{
	try {
		HT::HTHANDLE* StorageInstance;
		LPVOID MappinView;
		HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHTUpdate, (void**)&pIHTUpdate);
		if (SUCCEEDED(hr0))
		{
			HRESULT hr1 = pIHTUpdate->HTOpenMapView(MappingName, MappinView);
			StorageInstance = reinterpret_cast<HT::HTHANDLE*>(MappinView);
			if (SUCCEEDED(hr1))
			{
				for (;;)
				{
					StorageInstance->Addr = MappinView;
					std::mt19937 engine; // mt19937 как один из вариантов
					engine.seed(std::time(nullptr) ^ _getpid());
					int key = ((engine()) % 50) + 1;

					int newValue = 0;

					HT::Element* getel;
					pIHTUpdate->ElemCreate(&key, sizeof(key), NULL, sizeof(key), getel);
					HT::Element* elem;
					pIHTUpdate->HTGet(StorageInstance, getel, elem);

					BOOL res;
					if (elem != NULL)
					{
						newValue = *(int*)elem->payload + 1;
						if (pIHTUpdate->HTUpdate(StorageInstance, elem, &newValue, sizeof(newValue), res))
							std::cout << "HT_READ_UPDATE: key - " << key << ", value - " << 0 << ", newValue - " << newValue << "\n";
					}
					Sleep(1000);
				}
				pIHTInsert->Release();
			}
			else
			{
				pIHTInsert->Release();
				throw (int)hr1;
			}
		}
		else
		{

			throw (int)hr0;
		}
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}

void HTCOM_STATIC::Delete(OS13HANDLE h, const char MappingName[512])
{
	try {
		HT::HTHANDLE* StorageInstance;
		LPVOID MappinView;
		HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHTDelete, (void**)&pIHTDelete);
		if (SUCCEEDED(hr0))
		{
			HRESULT hr1 = pIHTDelete->HTOpenMapView(MappingName, MappinView);
			if (SUCCEEDED(hr1))
			{
				StorageInstance = reinterpret_cast<HT::HTHANDLE*>(MappinView);
				for (;;)
				{
					StorageInstance->Addr = MappinView;
					std::mt19937 engine; // mt19937 как один из вариантов
					engine.seed(std::time(nullptr)^_getpid());
					int key = ((engine()) % 50) + 1;

					HT::Element* elem;
					pIHTDelete->ElemCreate(&key, sizeof(key), NULL, sizeof(key), elem);
					BOOL res;
					std::cout << "HT_DELETE: key - " << key << "\n";
					pIHTDelete->HTDelete(StorageInstance, elem, res);
					Sleep(1000);
				}
				pIHTInsert->Release();
			}
			else
			{
				pIHTInsert->Release();
				throw (int)hr1;
			}
		}
		else
		{

			throw (int)hr0;
		}
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}

void HTCOM_STATIC::killProcessByName(const wchar_t* filename)
{
	try {
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof(pEntry);
		BOOL hRes = Process32First(hSnapShot, &pEntry);
		while (hRes)
		{
			if (wcscmp(pEntry.szExeFile, filename) == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
					(DWORD)pEntry.th32ProcessID);
				if (hProcess != NULL)
				{
					TerminateProcess(hProcess, 9);
					CloseHandle(hProcess);
				}
			}
			hRes = Process32Next(hSnapShot, &pEntry);
		}
		CloseHandle(hSnapShot);
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}