#pragma once
#include <vector>
#include <string>
#include "Tag.h"

class TagValidator
{
public:
	virtual std::vector<std::string> validateTags(std::vector<Tag*>) = 0;
	TagValidator();
	~TagValidator();
};

