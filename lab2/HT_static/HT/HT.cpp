#include <iostream>
#include "HT.h"

int main()
{
	setlocale(LC_CTYPE, "Russian");
	HT::HTHANDLE* ht = nullptr;
	const char* filename = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\HT\\HT\\HTspace.txt";
	try 
	{
		/*ht = HT::Create(
			1000,
			3000,
			10,
			226,
			filename);*/
		ht = HT::Open(filename);
		for (int i = 23; i <= 30; i++) {
			int key = i;
			int cvalue = key * 100;
			HT::Element* newEl = new HT::Element(&key, sizeof(key), &cvalue, sizeof(cvalue));
			HT::Insert(ht, newEl);
		}

		HT::Close(ht);
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
		if (ht != nullptr)
			std::cout << HT::GetLastError(ht);
	}
	return 0;
}


