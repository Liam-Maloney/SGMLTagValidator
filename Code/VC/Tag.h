#pragma once
#include <string>
#include <list>

class Tag
{
	std::string tagName;
	std::list<std::string> attributes;

public:
	Tag::Tag();
	Tag(std::string tagName, std::list<std::string> attributes);

};

