#include <iostream>
#include "json/parser.h"

int main()
{
	Init();
	std::cout << NeedRetcheck("lua_close") << std::endl; // just some testing
	std::cout << IsApartOfLua("lua_close") << std::endl; // just some testing
}
