#pragma once
#include <vector>
#include "Tag.h"

class TagParser
{
public:		
	virtual std::vector<Tag*> parseTagsFrom(std::string fileToParseFrom) = 0;
};

