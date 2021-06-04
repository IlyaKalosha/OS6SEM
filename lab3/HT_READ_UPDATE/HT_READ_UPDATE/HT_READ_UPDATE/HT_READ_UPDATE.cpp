#include <iostream>
#include "HT.h"
#include <ctime>

int main(int argc, char* argv[])
{
	try {
		LPVOID MappinView = HT::OpenMapView("FileMappingName");
		HT::HTHANDLE* StorageInstance = reinterpret_cast<HT::HTHANDLE*>(MappinView);
		for (;;)
		{
			StorageInstance->Addr = MappinView;
			int key;
			srand(time(0));
			key = (rand()+6) % 50;
			int value = key * 100;
			int newValue = 0;
			HT::Element* getel = new HT::Element(&key, sizeof(key));
			HT::Element* elem = HT::Get(StorageInstance, getel);
			if (elem != NULL)
			{
				newValue = *(int*)elem->payload + 1;
				if (HT::Update(StorageInstance, elem, &newValue, sizeof(newValue)))
					std::cout << "HT_READ_UPDATE: key - " << key << ", value - " << value << ", newValue - " << newValue << "\n";
			}
			Sleep(1000);
		}
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}