#pragma once
#include <string>
#include "XMLTagParser.h"
class TagParser;

class ParserFactory
{
public:
	TagParser* createParserFor(std::string fileType);
};

