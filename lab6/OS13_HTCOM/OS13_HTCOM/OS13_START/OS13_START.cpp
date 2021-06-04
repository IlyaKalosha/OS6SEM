// OS13_START.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma comment(lib, "OS13_HTCOM_STATIC.lib")
#include <iostream>
#include "OS13_HTCOM_STATIC.h"
#include "HT.h"

int main(int argc, char* argv[])
{
	try {
		OS13HANDLE h1 = HTCOM_STATIC::Init();
		int quit = 1;
		HT::HTHANDLE* Storage = NULL;
		if (argc == 2) 
		{
			Storage = HTCOM_STATIC::Start(h1, argv[1]);
		}
		else
		{
			Storage = HTCOM_STATIC::Start2(h1, argv[1], argv[2], argv[3]);
		}

		if (Storage != NULL) {
			std::cout << "HT_START: File opened and Mapping created\n";
			while (quit != 0)
			{
				std::cout << "quit - 0\n";
				std::cin >> quit;
				if (quit == 0)
				{
					//HTCOM_STATIC::Stop(h1,Storage);
					break;
				}
			}
		}
		HTCOM_STATIC::Dispose(h1);
		return 1;
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
		return 0;
	}

}
