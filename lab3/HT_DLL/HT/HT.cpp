//#include <iostream>
//#include "HT.h"
//
//int main()
//{
//	setlocale(LC_CTYPE, "Russian");
//	HT::HTHANDLE* ht = nullptr;
//	const char* filename = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab3\\Storage.txt";
//	try 
//	{
//		/*ht = HT::Create(
//			1000,
//			3000,
//			10,
//			226,
//			filename);*/
//		ht = HT::Open(filename);
//		HT::OpenMapView(filename);
//
//		HT::Close(ht);
//	}
//	catch (char * msg)
//	{
//		std::cout << msg << "\n";
//		if (ht != nullptr)
//			std::cout << HT::GetLastError(ht);
//	}
//	return 0;
//}
//
//
