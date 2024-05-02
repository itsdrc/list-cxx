#include<iostream>
#include "list.h"

int main()
{
	try {

		list<char> charlist;
		charlist.insert(charlist.end(), 'a');
		charlist.insert(charlist.end(), 'b');
		charlist.insert(charlist.end(), 'c');
		charlist.insert(charlist.end(), 'd');
		charlist.insert(charlist.end(), 'e');
		charlist.pop(charlist.begin());

		for (char x : charlist)
			std::cout << x << ' ';

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
}
