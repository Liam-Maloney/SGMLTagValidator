#pragma once
#include "TagParser.h"
#include "Tag.h"
#include "FileParser.h"
#include <string>

class XMLTagParser : public TagParser 
{

public:

	std::queue<Tag> parseTagsFrom(std::string fileToParseFrom);
	XMLTagParser();
	XMLTagParser(std::string testForNow);
	~XMLTagParser();
};