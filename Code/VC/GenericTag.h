#pragma once
#include <string>
#include <list>

class GenericTag
{

public:
	bool closing;
	bool reqClosing;

protected:

	std::string tagName;
	std::list<std::string> attributes;
	int lineNumber = 0;
};

