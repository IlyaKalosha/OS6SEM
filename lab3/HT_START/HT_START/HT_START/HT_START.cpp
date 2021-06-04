#include <iostream>
#include "HT.h"

int main(int argc, char* argv[])
{
	int quit = 1;
	HT::HTHANDLE* Storage = HT::Open("C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab3\\Storage.txt");
	if (Storage != NULL) {
		std::cout << "HT_START: File opened and Mapping created\n";
		while (quit != 0)
		{
			std::cout << "quit - 0\n";
			std::cin >> quit;
			if (quit == 0)
			{
				HT::Close(Storage);
				break;
			}
		}
	}
	
	return 1;
}