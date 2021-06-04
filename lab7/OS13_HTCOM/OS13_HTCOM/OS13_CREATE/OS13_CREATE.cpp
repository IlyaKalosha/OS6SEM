// OS13_CREATE.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma comment(lib, "OS13_HTCOM_STATIC.lib")
#include <iostream>
#include "OS13_HTCOM_STATIC.h"
#include "HT.h"

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	try {
		OS13HANDLE h1 = HTCOM_STATIC::Init();
		HT::HTHANDLE* storage = HTCOM_STATIC::Create(h1, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5], argv[6]);
		if (storage != NULL)
			std::cout << "HT-Storage Created Filename = " << storage->FileName <<
			", SnapshotInterval = " << storage->SecSnapshotInterval <<
			", Capacity = " << storage->Capacity <<
			", MaxKeyLength = " << storage->MaxKeyLength <<
			", MaxDataLength = " << storage->MaxPayloadLength<<
			", group = " << storage->HTUsersGroup;
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}
