#pragma once
#include "Tag.h"

class GenericTag
{

protected:

	std::string tagName;
	std::list<std::string> attributes;
	int lineNumber = 0;

};

