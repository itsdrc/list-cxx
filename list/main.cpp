#include<iostream>
#include "list.h"

int main()
{
	try
	{
		list<std::string> mystringlist;
		mystringlist.push_back("Hello World");
		mystringlist.push_back("Hello World 2");
		mystringlist.push_back("Hello World 3");
		mystringlist.push_back("Hello World 4");

		while (!mystringlist.empty())
		{
			std::cout << mystringlist.front() << '\n';
			mystringlist.pop_front();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
}
