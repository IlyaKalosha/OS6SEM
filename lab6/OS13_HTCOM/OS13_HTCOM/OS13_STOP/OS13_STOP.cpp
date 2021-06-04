#include <iostream>

#pragma comment(lib, "OS13_HTCOM_STATIC.lib")
#include <iostream>
#include "OS13_HTCOM_STATIC.h"
#include "HT.h"

int main(int argc, char* argv[])
{
	try {
		OS13HANDLE h1 = HTCOM_STATIC::Init();

		HTCOM_STATIC::Stop(h1, L"OS13_START.exe", L"OS13_INSERT.exe", L"OS13_UPDATE.exe", L"OS13_DELETE.exe");
		HTCOM_STATIC::Dispose(h1);
		return 1;
	}
	catch (char * msg)
	{
		std::cout << msg << "\n";
		return 0;
	}

}
