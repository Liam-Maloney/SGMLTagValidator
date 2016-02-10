#pragma once
#include <string>
#include "XMLTagParser.h"
class TagParser;

class ParserFactory
{
	TagParser* parserHandle;
public:
	TagParser* createParserFor(std::string fileType);
	~ParserFactory();
};

