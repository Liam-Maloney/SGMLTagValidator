#pragma once
#include <string>
#include <list>

class GenericTag
{

protected:

	std::string tagName;
	std::list<std::string> attributes;
	int lineNumber = 0;

};

