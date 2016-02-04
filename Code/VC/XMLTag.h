#pragma once
#include "Tag.h"

class XMLTag : public Tag
{
public:
	XMLTag();
	XMLTag(std::string tagName, std::list<std::string> attributes, int lineNumber);
	~XMLTag();
};

