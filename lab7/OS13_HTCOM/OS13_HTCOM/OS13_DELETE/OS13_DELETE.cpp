// OS13_DELETE.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma comment(lib, "OS13_HTCOM_STATIC.lib")
#include <iostream>
#include "OS13_HTCOM_STATIC.h"
#include "HT.h"

int main(int argc, char* argv[])
{
	try {
		OS13HANDLE h1 = HTCOM_STATIC::Init();

		HTCOM_STATIC::Delete(h1, "FileMappingName");

	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
	}
}

