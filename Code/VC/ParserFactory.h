#pragma once
#include "TagParser.h"
#include <string>

class ParserFactory
{
public:
	TagParser* createParserFor(std::string createAParserForThisFileType);
	ParserFactory();
	~ParserFactory();
};

