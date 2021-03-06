#include <iostream>
#include "HT.h"
#include <ctime>

int main(int argc, char* argv[])
{
	try {
		LPVOID MappinView = HT::OpenMapView(argv[1]);
		HT::HTHANDLE* StorageInstance = reinterpret_cast<HT::HTHANDLE*>(MappinView);
		for (;;)
		{
			StorageInstance->Addr = MappinView;

			int key;
			srand(time(0));
			key = (rand()+17) % 50;
			int value = key * 100;

			HT::Element* elem = new HT::Element(&key, sizeof(key));
			std::cout << "HT_DELETE: key - " << key << ", value - " << value << "\n";
			HT::Delete(StorageInstance, elem);
			Sleep(1000);
		}
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}
