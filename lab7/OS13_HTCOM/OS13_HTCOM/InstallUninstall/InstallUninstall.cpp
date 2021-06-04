// InstallUninstall.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define SERVICENAME L"testservice"
#define SERVICEPATH L"C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab7\\OS13_HTCOM\\OS13_HTCOM\Release\\OS15_SERVICE.exe"

#include <Windows.h>
#include <fstream>
#include <iostream>

char* errortxt(const char* msg, int code)
{
	char* buf = new char[512];

	sprintf_s(buf, 512, "%s: error code = %d\n", msg, code);

	return buf;
}




int InstallServiceC()
{
	SC_HANDLE schService = NULL, schSCManager = NULL;
	try
	{
		schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

		if (!schSCManager)
		{
			throw errortxt("OpenSCManager", GetLastError());
		}
		else
		{
			schService = CreateService(
				schSCManager,
				SERVICENAME,
				SERVICENAME,
				SERVICE_ALL_ACCESS,
				SERVICE_WIN32_SHARE_PROCESS,
				SERVICE_AUTO_START,
				SERVICE_ERROR_NORMAL,
				SERVICEPATH,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
			);

			if (!schService)
			{
				throw errortxt("CreateService", GetLastError());
			}
		}
	}
	catch (char* txt)
	{
		std::cout << txt << std::endl;
	}


	if (schSCManager)
	{
		CloseServiceHandle(schSCManager);
	}

	if (schService)
	{
		CloseServiceHandle(schService);
	}

	return 0;
}

int RemoveServiceC()
{
	SC_HANDLE schService = NULL, schSCManager = NULL;
	try
	{
		schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (!schSCManager)
		{
			throw errortxt("OpenSCManager", GetLastError());
		}

		schService = OpenService(schSCManager, SERVICENAME, SERVICE_ALL_ACCESS);
		if (!schService)
		{
			throw errortxt("OpenService", GetLastError());
		}

		if (!DeleteService(schService))
		{
			throw errortxt("DeleteService", GetLastError());
		}
	}
	catch (char* txt)
	{
		std::cout << txt << std::endl;
	}


	if (schSCManager)
	{
		CloseServiceHandle(schSCManager);
	}

	if (schService)
	{
		CloseServiceHandle(schService);
	}

	return 0;
}


int main(int argc, char* argv[])
{
	if (!strcmp(argv[argc - 1], "install"))
	{
	InstallServiceC();
	}
	else if (!strcmp(argv[argc - 1], "remove"))
	{
	RemoveServiceC();
	}
}

