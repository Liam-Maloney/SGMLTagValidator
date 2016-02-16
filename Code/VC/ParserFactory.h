#pragma once
#include <string>
#include "XMLTagParser.h"
#include "HTMLTagParser.h"
class ParserFactory
{
	TagParserInterface* parserHandle;
public:
	TagParserInterface* createParserFor(std::string fileType);
	~ParserFactory();
};

