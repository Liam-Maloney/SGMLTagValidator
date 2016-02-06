#pragma once
#include <list>
#include <string>
#include "Tag.h"

class TagValidator
{
public:
	virtual std::list<std::string> validateTags(std::list<Tag*>) = 0;
	TagValidator();
	~TagValidator();
};

