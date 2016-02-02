#pragma once
#include <string>
#include <list>

class Tag
{

	std::string tagName;
	std::list<std::string> attributes;
	int lineNumber = 0;

public:

	Tag::Tag();
	Tag(std::string tagName, std::list<std::string> attributes);

};

