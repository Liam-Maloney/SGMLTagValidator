#pragma once
#include <queue>
#include "Tag.h"

class TagParser
{
public:		
	virtual std::queue<Tag> parseTagsFrom(std::string fileToParseFrom) = 0;
};

