#pragma once
#include <string>
#include <vector>
#include "Attribute.h"
#include <vector>

class GenericTag
{

public:
	bool closing;
	bool reqClosing;

protected:

	std::string tagName;
	std::vector<Attribute> attributes;
	int lineNumber = 0;
};

