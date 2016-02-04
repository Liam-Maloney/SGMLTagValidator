#pragma once
#include "Tag.h"

class TagParser
{
public:		
	virtual std::list<Tag*> parseTagsFrom(std::string fileToParseFrom) = 0;
};

