// HT_CREATE.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "HT.h"

int main(int argc, char* argv[])
{
	try {
		HT::HTHANDLE* storage = HT::Create(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
		if (storage != NULL)
			std::cout << "HT-Storage Created Filename = " << storage->FileName <<
			", SnapshotInterval = " << storage->SecSnapshotInterval <<
			", Capacity = " << storage->Capacity <<
			", MaxKeyLength = " << storage->MaxKeyLength <<
			", MaxDataLength = " << storage->MaxPayloadLength;
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}
