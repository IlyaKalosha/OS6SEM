// OS13_INSERT.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma comment(lib, "OS13_HTCOM_STATIC.lib")
#include <iostream>
#include "OS13_HTCOM_STATIC.h"
#include "HT.h"

int main(int argc, char* argv[])
{
	try {
		OS13HANDLE h1 = HTCOM_STATIC::Init();

		HTCOM_STATIC::Insert(h1, argv[1]);
		
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}

